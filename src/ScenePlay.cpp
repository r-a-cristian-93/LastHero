#include "ScenePlay.h"
#include <cmath>
#include "SUpdate.h"
#include "SDraw.h"

ScenePlay::ScenePlay(Game* g, std::string lp)
	:Scene(g)
	,level_path(lp)
	,status_widget(nullptr)
	,score_widget(nullptr)
	,wave_widget(nullptr)
	,score(0)
	,wave_current(0)
	,wave_total(0)
{
	init();
}

void ScenePlay::init() {
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::A, Action::MOVE_LEFT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::D, Action::MOVE_RIGHT);
	game->act_mgr.registerAction(ActionManager::DEV_MOUSE, sf::Mouse::Left, Action::FIRE_PRIMARY);
	game->act_mgr.registerAction(ActionManager::DEV_MOUSE, sf::Mouse::Right, Action::FIRE_SECONDARY);

	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::P, Action::GAME_PAUSE);

	score_widget = interface.add();
	score_widget->setText("Score: ", game->assets->getFont(Assets::FONT_COURIER), 20);
	score_widget->text->setFillColor({255, 50, 50});
	score_widget->text->setPosition(10, 10);

	wave_widget = interface.add();
	wave_widget->setPosition(150,10);
	wave_widget->setSize(200,25);
	wave_widget->setBackground(game->assets->getTexture(Assets::TEX_FILL_DARK_GREEN), 10);
	wave_widget->setBorder(game->assets->getBorder(Assets::BORDER_SLICK));
	wave_widget->setText("Wave: ", game->assets->getFont(Assets::FONT_COURIER), 20);
	wave_widget->text->setFillColor({255, 255, 255});
	wave_widget->text->setPosition(180, 8);

	Widget* skull = new Widget();
	skull->setPosition(150,10);
	sf::Sprite& s = game->assets->getSprite(Assets::SPRITE_ICON_SKULL);
	skull->setBackground(s, sf::Vector2i(-10,-5));
	wave_widget->child = skull;

	spawnPlayer();
	load_level("res/level_001.cfg");
}

void ScenePlay::load_level(std::string path) {
	std::ifstream file(path);
	std::string word;

	int frame, recipe_id, pos_x, pos_y, dir_x, dir_y;
	size_t tag, code, type;

	ActionStream action_stream;

	while (file >> word) {
		if (word == "_ACT") {
			while (file >> word) {
				if (word == "_END") break;
				else if (word == "code") {
					file >> word;
					if (word == "spawn") code = Action::SPAWN_ENEMY;
				}
				else if (word == "type") {
					file >> word;
					if (word == "start") type = Action::TYPE_START;
					else if (word == "end") type = Action::TYPE_END;
				}
				else if (word == "frame") file >> frame;
				else if (word == "entity") {
					file >> word;
					if (word == "enemy") {
						tag = Entity::TAG_ENEMY;
						file >> recipe_id;
					}
				}
				else if (word == "pos") file >> pos_x >> pos_y;
				else if (word == "dir") file >> dir_x >> dir_y;
			}

			Action* action = new Action(code, type);
			action->ent_tag = new size_t(tag);
			action->ent_id = new size_t(recipe_id);
			action->pos = new sf::Vector2f(pos_x, pos_y);
			action->dir = new sf::Vector2f(dir_x, dir_y);

			action_stream << action;
		}
	}

	while (!action_stream.empty()) {
		Action* action;
		action_stream >> action;
		doAction(action);
	}
}

void ScenePlay::update() {
	sf::FloatRect lim(0, 0, game->app_conf.window_w, game->app_conf.window_h);

	if (!paused) {
		ent_mgr.update();
		//sEnemySpawner();
		sLifespan();
		sMissleGuidance();
		SUpdate::updatePosition(ent_mgr.getEntities(), lim);
		sCollision();
		sCombat();
	}
	sSpin();

	//update score_widget
	score_text = "Score: " + std::to_string(score);
	score_widget->setText(score_text);

	//update wave_widget
	wave_text = "Wave: " + std::to_string(wave_current) + " of " + std::to_string(wave_total);
	wave_widget->setText(wave_text);

	SDraw::drawEntities(&game->window, ent_mgr.getEntities());
	SDraw::drawInterface(&game->window, interface.getWidgets());

	frame_current++;
}

void ScenePlay::spawnPlayer() {
	const sf::Vector2f pos(game->app_conf.window_w/2, game->app_conf.window_h/2);

	player = ent_mgr.add(Entity::TAG_PLAYER);

	player->get<CTransform>()->pos = pos;
	player->get<CShape>()->shape.setPosition(pos);
}

void ScenePlay::spawnEnemy() {
	const sf::Vector2f dir(rand(), rand());
	bool position_is_valid = false;
	sf::Vector2f pos;
	int player_radius = player->get<CCollision>()->radius;

	std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_ENEMY, 0);
	int radius = e->get<CCollision>()->radius;

	while (!position_is_valid) {
		pos.x = rand() % static_cast<int>(game->app_conf.window_w - radius*2) + radius;
		pos.y = rand() % static_cast<int>(game->app_conf.window_h - radius*2) + radius;

		float square_min_dist = (player_radius*10 + radius) * (player_radius*10 + radius);
		float square_current_dist = squareDistance(pos, player->get<CTransform>()->pos);

		if (square_current_dist > square_min_dist) {
			position_is_valid = true;
		}
	}

	e->get<CTransform>()->pos = pos;
	e->get<CTransform>()->dir = dir;
	e->get<CShape>()->shape.setPosition(pos);
}

void ScenePlay::spawnEnemy(size_t tag, size_t recipe_id, sf::Vector2f pos, sf::Vector2f dir) {
	int player_radius = player->get<CCollision>()->radius;

	std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_ENEMY, recipe_id);
	int radius = e->get<CCollision>()->radius;

	e->get<CTransform>()->pos = pos;
	e->get<CTransform>()->dir = dir;
	e->get<CShape>()->shape.setPosition(pos);
}

void ScenePlay::sEnemySpawner() {
	if (frame_current % 100 == 0) {
		spawnEnemy();
	}
}

bool ScenePlay::checkCollision(std::shared_ptr<Entity>& a, std::shared_ptr<Entity>& b) {
	float square_distance = squareDistance(a->get<CTransform>()->pos, b->get<CTransform>()->pos);

	if (a->get<CCollision>() && b->get<CCollision>()) {
		int square_radius =
			(a->get<CCollision>()->radius + b->get<CCollision>()->radius) *
			(a->get<CCollision>()->radius + b->get<CCollision>()->radius);

		if (square_distance < square_radius) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}


void ScenePlay::sCollision() {
	bool collision = false;

	//what kills player
	for (std::shared_ptr<Entity>& enemy : ent_mgr.getEntities(Entity::TAG_ENEMY)) {
		collision = checkCollision(player, enemy);
		if (collision) {
			player->alive = false;
			score /= 2;
			spawnPlayer();
		}
	}

	for (std::shared_ptr<Entity>& child : ent_mgr.getEntities(Entity::TAG_CHILD)) {
		collision = checkCollision(player, child);
		if (collision) {
			player->alive = false;
			score /= 2;
			spawnPlayer();
		}
	}

	//what kills enemies
	for (std::shared_ptr<Entity>& bullet : ent_mgr.getEntities(Entity::TAG_BULLET)) {
		for (std::shared_ptr<Entity>& enemy : ent_mgr.getEntities(Entity::TAG_ENEMY)) {
			collision = checkCollision(bullet, enemy);
			if (collision) {
				enemy->alive = false;
				bullet->alive = false;
				spawnChilds(enemy);

				if (enemy->get<CScore>()) {
					score += enemy->get<CScore>()->score;
				}

			}
		}

		for (std::shared_ptr<Entity>& child : ent_mgr.getEntities(Entity::TAG_CHILD)) {
			collision = checkCollision(bullet, child);
			if (collision) {
				child->alive = false;
				bullet->alive = false;

				if (child->get<CScore>()) {
					score += child->get<CScore>()->score;
				}
			}
		}
	}

	for (std::shared_ptr<Entity>& missle : ent_mgr.getEntities(Entity::TAG_MISSLE)) {
		for (std::shared_ptr<Entity>& enemy : ent_mgr.getEntities(Entity::TAG_ENEMY)) {
			collision = checkCollision(missle, enemy);
			if (collision) {
				enemy->alive = false;
				missle->alive = false;

				if (enemy->get<CScore>()) {
					score += enemy->get<CScore>()->score;
				}
			}
		}

		for (std::shared_ptr<Entity>& child : ent_mgr.getEntities(Entity::TAG_CHILD)) {
			collision = checkCollision(missle, child);
			if (collision) {
				child->alive = false;

				if (child->get<CScore>()) {
					score += child->get<CScore>()->score;
				}
			}
		}
	}
}

void ScenePlay::spawnChilds(const std::shared_ptr<Entity>& parent) {
	const size_t vertices = parent->get<CShape>()->shape.getPointCount();
	const float rotation = parent->get<CShape>()->shape.getRotation();
	const float alpha = 360 / vertices;
	const float radius = parent->get<CShape>()->shape.getRadius() / vertices;
	const float vel = parent->get<CTransform>()->max_velocity / 2;
	const int lifespan = static_cast<int>(radius) * 6;
	const sf::Color fill_color = parent->get<CShape>()->shape.getFillColor();
	sf::Vector2f dir;
	sf::Vector2f pos;

	for (size_t i=0; i<vertices; i++) {
		dir.x = cos((alpha*i + rotation) * PI / 180);
		dir.y = sin((alpha*i + rotation) * PI / 180);
		pos = parent->get<CTransform>()->pos + dir*(radius*2);

		CShape* c_shape = new CShape(sf::CircleShape(radius, vertices));
		sf::CircleShape& shape = c_shape->shape;
		shape.setOrigin(radius, radius);
		shape.setFillColor(fill_color);
		shape.setPosition(pos);

		std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_CHILD);

		e->add<CTransform>(new CTransform(pos, dir, vel));
		e->get<CTransform>()->d_angle = vel;
		e->add<CShape>(c_shape);
		e->add<CCollision>(new CCollision(radius));
		e->add<CLifespan>(new CLifespan(lifespan));
		e->alive = true;
	}
}

void ScenePlay::sCombat() {
	if (player->get<CInput>()->fire_primary) {
		spawnBullet();
		player->get<CInput>()->fire_primary = false;
	}
	if (player->get<CInput>()->fire_secondary) {
		if (ent_mgr.getEntities(Entity::TAG_MISSLE).size() == 0) {
			spawnMissle();
		}
		player->get<CInput>()->fire_secondary = false;
	}
}

void ScenePlay::sLifespan() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities(Entity::TAG_BULLET)) {
		checkLifespan(e);
	}

	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities(Entity::TAG_CHILD)) {
		checkLifespan(e);
	}
}

void ScenePlay::checkLifespan(std::shared_ptr<Entity>& e) {
	if (e->get<CLifespan>()) {
		const int lifespan = e->get<CLifespan>()->lifespan;
		int& remaining = e->get<CLifespan>()->remaining;

		remaining--;

		if (remaining * 3 < lifespan) {
			int alpha = remaining * 255 / lifespan*3;

			sf::Color color = e->get<CShape>()->shape.getFillColor();
			color.a = alpha;
			e->get<CShape>()->shape.setFillColor(color);
		}

		if (remaining <= 0) {
			e->alive = false;
		}
	}
}

void ScenePlay::spawnBullet() {
	sf::Vector2f mouse_pos;

	mouse_pos = sf::Vector2f(sf::Mouse::getPosition(game->window));


	const sf::Vector2f pos(player->get<CTransform>()->pos);
	const sf::Vector2f dir = mouse_pos - pos;

	std::shared_ptr<Entity> bullet = ent_mgr.add(Entity::TAG_BULLET);

	bullet->get<CTransform>()->pos = pos;
	bullet->get<CTransform>()->dir = dir;
	bullet->get<CShape>()->shape.setPosition(pos);
}

void ScenePlay::sSpin() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities()) {
		if (e->get<CTransform>()) {
			if (e->get<CTransform>()->d_angle) {
				e->get<CShape>()->shape.rotate(e->get<CTransform>()->d_angle);
			}
		}
	}
}

void ScenePlay::spawnMissle() {
	sf::Vector2f mouse_pos;

	mouse_pos = sf::Vector2f(sf::Mouse::getPosition(game->window));

	const sf::Vector2f pos(player->get<CTransform>()->pos);
	const sf::Vector2f dir = mouse_pos - pos;

	std::shared_ptr<Entity> missle = ent_mgr.add(Entity::TAG_MISSLE);

	missle->get<CTransform>()->pos = pos;
	missle->get<CTransform>()->dir = dir;
	missle->get<CShape>()->shape.setPosition(pos);
}

std::shared_ptr<Entity> ScenePlay::findTarget(const std::shared_ptr<Entity>& missle) {
	EntityVec reachable;
	sf::Vector2f dir_missle(missle->get<CTransform>()->dir);
	sf::Vector2f dir_enemy;

	for (std::shared_ptr<Entity>& enemy : ent_mgr.getEntities(Entity::TAG_ENEMY)) {
		dir_enemy = enemy->get<CTransform>()->pos - missle->get<CTransform>()->pos;

		if (angle(dir_missle, dir_enemy) < 30) {
			reachable.push_back(enemy);
		}
	}

	std::shared_ptr<Entity> target;
	float prev_dist(game->app_conf.window_w*game->app_conf.window_w);
	float dist;

	for (std::shared_ptr<Entity>& enemy : reachable) {
		dist = abs(squareDistance(missle->get<CTransform>()->pos, enemy->get<CTransform>()->pos));
		if (dist < prev_dist) {
			target = enemy;
			prev_dist = dist;
		}
	}

	if (target) {
		return target;
	}
	else {
		return nullptr;
	}
}

void ScenePlay::sMissleGuidance() {
	for (std::shared_ptr<Entity>& missle : ent_mgr.getEntities(Entity::TAG_MISSLE)) {

		std::shared_ptr<Entity> new_target = findTarget(missle);
		std::shared_ptr<Entity> prev_target = nullptr;
		bool set_new_target = false;

		if (missle->get<CTarget>()->target) {
			prev_target = missle->get<CTarget>()->target;
		}

		if (!new_target) {
			set_new_target = false;
		}
		else {
			if (prev_target) {
				if (prev_target->id == new_target->id) {
					set_new_target = false;
				}
				else {
					set_new_target = true;
				}
			}
			else {
				set_new_target = true;
			}

			if (set_new_target) {
				if (new_target) {
					missle->get<CTarget>()->target = new_target;
				}
			}
		}

		if (set_new_target && missle->get<CTarget>()) {
			if (missle->get<CTarget>()->target) {
				sf::Vector2f target_pos = missle->get<CTarget>()->target->get<CTransform>()->pos;
				sf::Vector2f missle_pos = missle->get<CTransform>()->pos;
				sf::Vector2f dir(target_pos - missle_pos);

				missle->get<CTransform>()->dir = dir;
			}
		}
	}
}

float ScenePlay::angle(const sf::Vector2f a, const sf::Vector2f b) {
	float dot_a_b = a.x*b.x + a.y*b.y;
	float mod_a_b = sqrt((a.x*a.x + a.y*a.y) * (b.x*b.x + b.y*b.y));

	return acos(dot_a_b / mod_a_b) / PI * 180;
}

void ScenePlay::doAction(const Action* a) {
	if (*a->type == Action::TYPE_START) {
		switch (*a->code) {
			case Action::MOVE_UP:
				player->get<CInput>()->up = true;
			break;
			case Action::MOVE_LEFT:
				player->get<CInput>()->left = true;
			break;
			case Action::MOVE_DOWN:
				player->get<CInput>()->down = true;
			break;
			case Action::MOVE_RIGHT:
				player->get<CInput>()->right = true;
			break;
			case Action::FIRE_PRIMARY:
				player->get<CInput>()->fire_primary = true;
			break;
			case Action::FIRE_SECONDARY:
				player->get<CInput>()->fire_secondary = true;
			break;
			case Action::GAME_PAUSE:
				paused = !paused;
			break;
			case Action::SPAWN_ENEMY:
				spawnEnemy(*a->ent_tag, *a->ent_id, *a->pos, *a->dir);
			break;
			default:
			break;
		}
	}
	if (*a->type == Action::TYPE_END) {
		switch (*a->code) {
			case Action::MOVE_UP:
				player->get<CInput>()->up = false;
			break;
			case Action::MOVE_LEFT:
				player->get<CInput>()->left = false;
			break;
			case Action::MOVE_DOWN:
				player->get<CInput>()->down = false;
			break;
			case Action::MOVE_RIGHT:
				player->get<CInput>()->right = false;
			break;
			default:
			break;
		}
	}
}

float ScenePlay::squareDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}
