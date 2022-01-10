#include "ScenePlay.h"
#include <cmath>
#include "SUpdate.h"
#include "SDraw.h"

ScenePlay::ScenePlay(Game* g, std::string lp)
	:Scene(g)
	,level_path(lp)
{
	init();
}

void ScenePlay::init() {
	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::A, Action::MOVE_LEFT);
	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);
	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::D, Action::MOVE_RIGHT);
	act_mgr.registerAction(ActionManager::DEV_MOUSE, sf::Mouse::Left, Action::FIRE_PRIMARY);
	act_mgr.registerAction(ActionManager::DEV_MOUSE, sf::Mouse::Right, Action::FIRE_SECONDARY);

	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::P, Action::GAME_PAUSE);
	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::F1, Action::REPLAY_SAVE);
	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::F2, Action::REPLAY_START);
	
	load_level();
}

void ScenePlay::load_level() {


}

void ScenePlay::update() {
	sf::FloatRect lim(0, 0, game->app_conf.window_w, game->app_conf.window_h);
	
	if (!paused) {
		ent_mgr.update();
		sEnemySpawner();
		sLifespan();
		sMissleGuidance();
		SUpdate::updatePosition(ent_mgr.getEntities(), lim);
		sCollision();
		sCombat();
	}

	sSpin();
	SDraw::drawEntities(&game->window, ent_mgr.getEntities());


	std::string sc = std::to_string(score);
	score_text.setString(sc);
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

void ScenePlay::sEnemySpawner() {
	if (frame_current % 100 == 0) {
		spawnEnemy();
	}
}

void ScenePlay::sUserInput() {
	sf::Event event;
	Action* action = nullptr;
	int action_code = Action::NONE;

	while (game->window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			game->running = false;
			game->window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			action_code = act_mgr.getCode(ActionManager::DEV_KEYBOARD, event.key.code);

			if (action_code) {
				action = new Action(action_code, Action::TYPE_START, frame_current);
				doAction(action);
			}
		}
		else if (event.type == sf::Event::KeyReleased) {
			action_code = act_mgr.getCode(ActionManager::DEV_KEYBOARD, event.key.code);

			if (action_code) {
				action = new Action(action_code, Action::TYPE_END, frame_current);
				doAction(action);
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed && !paused){
			action_code = act_mgr.getCode(ActionManager::DEV_MOUSE, event.mouseButton.button);

			if (action_code != 0) {
				action = new Action(action_code, Action::TYPE_START, frame_current, sf::Mouse::getPosition());
				doAction(action);
			}
		}
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
