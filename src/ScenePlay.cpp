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
	,bg_tex(nullptr)
{
	init();
}
ScenePlay::~ScenePlay() {
	delete bg_tex;
}

void ScenePlay::init() {
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::A, Action::MOVE_LEFT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::D, Action::MOVE_RIGHT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Space, Action::FIRE_PRIMARY);
	game->act_mgr.registerAction(ActionManager::DEV_MOUSE, sf::Mouse::Left, Action::FIRE_PRIMARY);
	game->act_mgr.registerAction(ActionManager::DEV_MOUSE, sf::Mouse::Right, Action::FIRE_SECONDARY);

	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::P, Action::GAME_PAUSE);

	spawnPlayer();
	load_level("res/level_001.cfg");

	// health widget
	WidgetBox* w_health_ico = new WidgetBox();
	w_health_ico->setSize(sf::Vector2i(40,40));
	w_health_ico->setPosRel(sf::Vector2i(-10,-8));
	w_health_ico->setBackground(game->assets->getSprite("icon_hart"), sf::Vector2i(0,0));

	w_health_text = new WidgetText();
	w_health_text->setPosRel(sf::Vector2i(40, -2));
	int health = player->get<CStats>()->effective[CStats::HEALTH];
	w_health_text->setText(std::to_string(health), game->assets->getFont(Assets::FONT_COURIER), 20);

	WidgetBox* w_health = new WidgetBox();
	w_health->setSize(sf::Vector2i(100,25));
	w_health->setPosAbs(sf::Vector2i(20,10));
	w_health->setBackground(game->assets->getTexture("dark_green"), 10);
	w_health->setBorder(game->assets->getBorder("slick"));
	w_health->addChild(w_health_ico);
	w_health->addChild(w_health_text);

	// defence widget
	WidgetBox* w_defence_ico = new WidgetBox();
	w_defence_ico->setSize(sf::Vector2i(40,40));
	w_defence_ico->setPosRel(sf::Vector2i(-10,-8));
	w_defence_ico->setBackground(game->assets->getSprite("icon_helmet"), sf::Vector2i(0,0));

	w_defence_text = new WidgetText();
	w_defence_text->setPosRel(sf::Vector2i(40, -2));
	int defence = player->get<CStats>()->effective[CStats::DEFENCE];
	w_defence_text->setText(std::to_string(defence), game->assets->getFont(Assets::FONT_COURIER), 20);

	WidgetBox* w_defence = new WidgetBox();
	w_defence->setSize(sf::Vector2i(100,25));
	w_defence->setPosAbs(sf::Vector2i(150,10));
	w_defence->setBackground(game->assets->getTexture("dark_green"), 10);
	w_defence->setBorder(game->assets->getBorder("slick"));
	w_defence->addChild(w_defence_ico);
	w_defence->addChild(w_defence_text);

	// waves widget
	WidgetBox* w_waves_ico = new WidgetBox();
	w_waves_ico->setSize(sf::Vector2i(40,40));
	w_waves_ico->setPosRel(sf::Vector2i(-10,-8));
	w_waves_ico->setBackground(game->assets->getSprite("icon_skull"), sf::Vector2i(0,0));

	WidgetBox* w_waves = new WidgetBox();
	w_waves->setSize(sf::Vector2i(100,25));
	w_waves->setPosAbs(sf::Vector2i(280,10));
	w_waves->setBackground(game->assets->getTexture("dark_green"), 10);
	w_waves->setBorder(game->assets->getBorder("slick"));
	w_waves->addChild(w_waves_ico);

	interface.add(w_health);
	interface.add(w_defence);
	interface.add(w_waves);

	gui_view.reset(sf::FloatRect(0 ,0, game->app_conf.game_w, game->app_conf.game_h));
}

void ScenePlay::load_level(std::string path) {
	std::ifstream file(path);
	std::string word;

	sf::IntRect bg_rect(0,0,0,0);

	int frame, pos_x, pos_y, dir_x, dir_y;
	std::string enemy_name;
	size_t tag, code, type;

	ActionStream action_stream;

	while (file >> word) {
		if (word == "_HEADER") {
			while (file>>word) {
				if (word == "_END") break;
				else if (word == "size")	{
					file >> bg_rect.width >> bg_rect.height;
				}
				else if (word == "background") {
					file >> word;
					bg_tex = new sf::Texture();
					bg_tex->loadFromFile(word, bg_rect);
					bg_sprite = sf::Sprite(*bg_tex);
				}
			}
		}
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
						file >> enemy_name;
					}
				}
				else if (word == "pos") file >> pos_x >> pos_y;
				else if (word == "dir") file >> dir_x >> dir_y;
			}

			Action* action = new Action(code, type);
			action->ent_tag = new size_t(tag);
			action->ent_name = new std::string(enemy_name);
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
	sf::FloatRect lim(bg_sprite.getLocalBounds());

	if (!paused) {
		ent_mgr.update();
		sEnemySpawner();
		sLevelUp();
		sLifespan();
		sMissleGuidance();
		SUpdate::updatePosition(ent_mgr.getEntities(), lim);
		sCollision();
		sCombat();
		sInterface();
	}
	sSpin();
	sAnimation();
	sView();

	game->screen_tex.draw(bg_sprite);

	SDraw::drawEntities(&game->screen_tex, ent_mgr.getEntities());


	if (glitter.m_lifetime >=0) {
		glitter.update();
		game->screen_tex.draw(glitter);
	}

	//change view in order to keep the interface relative to window
	game->screen_tex.setView(gui_view);
	SDraw::drawInterface(&game->screen_tex, interface.getWidgets());
	game->screen_tex.setView(game->view);

	frame_current++;
}

void ScenePlay::spawnPlayer() {
	const sf::Vector2f pos(game->app_conf.window_w/2, game->app_conf.window_h/2);


	std::string r_name = "cowboy";
	player = ent_mgr.add(Entity::TAG_PLAYER);

	player->get<CTransform>()->pos = pos;
	player->get<CShape>()->shape.setPosition(pos + player->get<CCollision>()->offset[player->facing]);

	setStatsInitial(*player);
	setStatsEffective(*player);
}

void ScenePlay::spawnEnemy() {
	sf::Vector2f dir(rand()%2, rand()%2);
	if (rand()%2 == 0) dir.x *= -1;
	if (rand()%2 == 0) dir.y *= -1;

	bool position_is_valid = false;
	sf::Vector2f pos;
	int player_radius = player->get<CCollision>()->radius;

	std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_ENEMY);
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
	e->get<CShape>()->shape.setPosition(pos + e->get<CCollision>()->offset[e->facing]);
	e->get<CStats>()->level = rand() % 10;

	setStatsInitial(*e);
	setStatsEffective(*e);
}

void ScenePlay::spawnEnemy(size_t tag, std::string& recipe_name, sf::Vector2f& pos, sf::Vector2f& dir) {
	int player_radius = player->get<CCollision>()->radius;

	std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_ENEMY, recipe_name);

	e->get<CTransform>()->pos = pos;
	e->get<CTransform>()->dir = dir;
	e->get<CShape>()->shape.setPosition(pos + e->get<CCollision>()->offset[e->facing]);

	setStatsInitial(*e);
	setStatsEffective(*e);
}

void ScenePlay::sEnemySpawner() {
	if (frame_current % 100 == 0) {
		spawnEnemy();
	}
}

bool ScenePlay::checkCollision(std::shared_ptr<Entity>& a, std::shared_ptr<Entity>& b) {
	if (a->get<CCollision>() && b->get<CCollision>()) {
		sf::Vector2f pos_a = a->get<CTransform>()->pos + a->get<CCollision>()->offset[a->facing];
		sf::Vector2f pos_b = b->get<CTransform>()->pos + b->get<CCollision>()->offset[b->facing];

		float square_distance = squareDistance(pos_a, pos_b);

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
			enemy->alive = false;
			spawnChilds(enemy);

			int enemy_atk = enemy->get<CStats>()->effective[CStats::ATTACK];
			int& player_hp = player->get<CStats>()->effective[CStats::HEALTH];
			int& player_def = player->get<CStats>()->effective[CStats::DEFENCE];

			if (player_def > 0) {
				player_def -= enemy_atk;

				if (player_def < 0) {
					player_hp -= -player_def;
					player_def = 0;
				}
			}
			else {
				player_hp -= enemy_atk;
			}

			if (player_hp <= 0) {
				player->alive = false;
				spawnPlayer();
			}
		}
	}

	for (std::shared_ptr<Entity>& child : ent_mgr.getEntities(Entity::TAG_CHILD)) {
		collision = checkCollision(player, child);
		if (collision) {
			child->alive = false;

			int child_atk = child->get<CStats>()->effective[CStats::ATTACK];
			int& player_hp = player->get<CStats>()->effective[CStats::HEALTH];
			int& player_def = player->get<CStats>()->effective[CStats::DEFENCE];

			if (player_def > 0) {
				player_def -= child_atk;

				if (player_def < 0) {
					player_hp -= -player_def;
					player_def = 0;
				}
			}
			else {
				player_hp -= child_atk;
			}

			if (player_hp <= 0) {
				player->alive = false;
				spawnPlayer();
			}
		}
	}

	//what kills enemies
	for (std::shared_ptr<Entity>& projectile : ent_mgr.getEntities(Entity::TAG_PROJECTILE)) {
		for (std::shared_ptr<Entity>& enemy : ent_mgr.getEntities(Entity::TAG_ENEMY)) {
			collision = checkCollision(projectile, enemy);
			if (collision) {
				enemy->alive = false;
				projectile->alive = false;
				sf::Vector2f g_pos = projectile->get<CTransform>()->pos;

				spawnExplosion(g_pos);

				if (enemy->get<CScore>()) {
					score += enemy->get<CScore>()->score;
				}

			}
		}

		for (std::shared_ptr<Entity>& child : ent_mgr.getEntities(Entity::TAG_CHILD)) {
			collision = checkCollision(projectile, child);
			if (collision) {
				child->alive = false;
				projectile->alive = false;

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

		CStats* c_stats = new CStats();
		c_stats->effective[CStats::ATTACK] = vertices;
		std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_CHILD);

		e->add<CTransform>(new CTransform(pos, dir, vel));
		e->get<CTransform>()->d_angle = vel;
		e->add<CShape>(c_shape);
		e->add<CCollision>(new CCollision(radius));
		e->add<CLifespan>(new CLifespan(lifespan));
		e->add<CStats>(c_stats);
		e->alive = true;
	}
}

void ScenePlay::spawnExplosion(sf::Vector2f& pos) {
	std::string recipe = "glowing_bullet_explosion";
	std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_SFX, recipe);

	e->add<CTransform>(new CTransform(pos, 0));

	e->state = Entity::STATE_DIE;
	e->alive = true;
}

void ScenePlay::sCombat() {
	if (player->get<CInput>()->fire_primary) {
		if (player->get<CWeapon>()) {
			spawnBullet();
		}
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
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities(Entity::TAG_PROJECTILE)) {
		checkLifespan(e);
	}

	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities(Entity::TAG_CHILD)) {
		checkLifespan(e);
	}

	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities(Entity::TAG_SFX)) {
		checkLifespan(e);
	}
}

void ScenePlay::checkLifespan(std::shared_ptr<Entity>& e) {
	if (e->get<CLifespan>()) {
		const int lifespan = e->get<CLifespan>()->lifespan;
		int& remaining = e->get<CLifespan>()->remaining;

		remaining--;

		if (remaining * 3 < lifespan) {
			if (e->get<CShape>()) {
				sf::Color color = e->get<CShape>()->shape.getFillColor();
				color.a = remaining * color.a / lifespan*3;

				e->get<CShape>()->shape.setFillColor(color);
			}

			if (e->get<CAnimation>()) {
				sf::Color color = e->get<CAnimation>()->anim_set.color_mod;
				color.a = remaining * color.a / lifespan*3;

				e->get<CAnimation>()->anim_set.setColorMod(color);
			}
		}

		if (remaining <= 0) {
			e->alive = false;
		}
	}
}

void ScenePlay::spawnBullet() {
	const sf::Vector2f pos(player->get<CTransform>()->pos);
	const sf::Vector2f dir(player->get<CTransform>()->prev_dir);
	const sf::Vector2f offset(player->get<CWeapon>()->projectile_spawn[player->facing]);

	std::string& r_name = player->get<CWeapon>()->primary;
	std::shared_ptr<Entity> bullet = ent_mgr.add(Entity::TAG_PROJECTILE, r_name);

	bullet->get<CTransform>()->pos = pos + offset;
	bullet->get<CTransform>()->dir = dir;
	bullet->get<CShape>()->shape.setPosition(pos + offset);
}

void ScenePlay::sSpin() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities()) {
		if (e->get<CTransform>() && e->get<CShape>()) {
			if (e->get<CTransform>()->d_angle) {
				e->get<CShape>()->shape.rotate(e->get<CTransform>()->d_angle);
			}
		}
	}
}

void ScenePlay::spawnMissle() {
	sf::Vector2f mouse_pos = game->window.mapPixelToCoords(sf::Mouse::getPosition(game->window));

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
				spawnEnemy(*a->ent_tag, *a->ent_name, *a->pos, *a->dir);
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


void ScenePlay::setStatsInitial(Entity& entity) {
	CStats& stats = *entity.get<CStats>();
	for (int i=0; i<CStats::COUNT; i++) {
		stats.initial[i] = stats.base[i] + stats.per_level[i] * stats.level;
	}
}

void ScenePlay::setStatsEffective(Entity& entity) {
	CStats& stats = *entity.get<CStats>();
	for (int i=0; i<CStats::COUNT; i++) {
		stats.effective[i] = stats.initial[i];
	}
}

void ScenePlay::sLevelUp() {

}

void ScenePlay::sInterface() {
	if (player) {
		if (w_health_text) {
			int health = player->get<CStats>()->effective[CStats::HEALTH];
			w_health_text->setText(std::to_string(health));
		}
		if (w_defence_text) {
			int defence = player->get<CStats>()->effective[CStats::DEFENCE];
			w_defence_text->setText(std::to_string(defence));
		}
	}
}

void ScenePlay::sAnimation() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities()) {
		if (e->get<CAnimation>() && e->facing != 0) {
			e->get<CAnimation>()->active_anim->update();
			if (e->get<CAnimation>()->active_anim->hasEnded()) {
				if (e->get<CTransform>()) {
					sf::Vector2f e_pos(e->get<CTransform>()->pos);
					sf::Vector2f e_dir(e_pos + e->get<CTransform>()->prev_dir);
					size_t max_directions = e->get<CAnimation>()->anim_set.animations[e->state].size();

					float c1, c2;
					c1 = e_dir.y - e_pos.y;
					c2 = e_dir.x - e_pos.x;

					float deg = - ((atan2(-c1, -c2)/ PI * 180 ) - 180);
					float facing = ceil( deg*max_directions/360.0f + 0.5f );	// float facing = ceil(  (deg + 180/max_directions) / (360/max_directions)  );

					if (facing > max_directions) facing = 1;

					e->facing = facing;

					if (e->get<CAnimation>()->active_anim != &e->get<CAnimation>()->anim_set.animations[e->state][facing]) {
						e->get<CAnimation>()->active_anim = &e->get<CAnimation>()->anim_set.animations[e->state][facing];
					}
				}
			}

			// update animation sprite position
			if (e->get<CTransform>()) {
				e->get<CAnimation>()->active_anim->getSprite().setPosition(e->get<CTransform>()->pos);
			}
		}
	}
}

void ScenePlay::sView() {
	//update camera position
	cam.target = player->get<CTransform>()->pos;
	float square_delta = squareDistance(cam.pos, cam.target);

	if (square_delta > game->app_conf.cam_treshold) {
		cam.pos += ((cam.target - cam.pos) / game->app_conf.cam_speed);
	}

	//update view position
	int w = game->app_conf.game_w;
	int h = game->app_conf.game_h;
	sf::FloatRect world(bg_sprite.getLocalBounds());
	sf::FloatRect rect(cam.pos.x-w/2, cam.pos.y-h/2, w, h);

	if (rect.left < 0) rect.left = 0;
	if (rect.top < 0) rect.top = 0;
	if (rect.left + rect.width > world.width) rect.left = world.width - w;
	if (rect.top + rect.height > world.height) rect.top = world.height - h;

	game->view.reset(rect);
}

float ScenePlay::squareDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}
