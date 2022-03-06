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
ScenePlay::~ScenePlay() {}

void ScenePlay::init() {
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::A, Action::MOVE_LEFT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::D, Action::MOVE_RIGHT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Space, Action::FIRE_PRIMARY);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::N, Action::FIRE_PRIMARY);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::M, Action::FIRE_SECONDARY);
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

	sf::Vector2u tile_size(0, 0), map_size(0, 0);
	std::string texture_name = "";
	int* level_layer;

	int frame, pos_x, pos_y, dir_x, dir_y;
	std::string enemy_name;
	size_t tag, code, type, state, facing;

	ActionStream action_stream;

	while (file >> word) {
		if (word == "_HEADER") {
			while (file>>word) {
				if (word == "_END") break;
				else if (word == "size")	{
					file >> map_size.x >> map_size.y;
					level_layer = new int[map_size.x*map_size.y];
				}
				else if (word == "tile_size") {
					file >> tile_size.x >> tile_size.y;
				}
				else if (word == "texture") {
					file >> texture_name;
				}
			}
		}
		if (word == "_LAYER") {
			for (int t = 0; t < map_size.x*map_size.y; t++) {
				file >> level_layer[t];
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
				else if (word == "state") {
					file >> word;
					if (word == "idle") state = Entity::STATE_IDLE;
					else if (word == "run") state = Entity::STATE_RUN;
					else if (word == "die") state = Entity::STATE_DIE;
				}
				else if (word == "facing") {
					file >> word;
					if (word == "E") facing = Entity::FACING_E;
					else if (word == "NE") facing = Entity::FACING_NE;
					else if (word == "N") facing = Entity::FACING_N;
					else if (word == "NW") facing = Entity::FACING_NW;
					else if (word == "W") facing = Entity::FACING_W;
					else if (word == "SW") facing = Entity::FACING_SW;
					else if (word == "S") facing = Entity::FACING_S;
					else if (word == "SE") facing = Entity::FACING_SE;
				}
			}

			Action* action = new Action(code, type);
			action->ent_tag = new size_t(tag);
			action->ent_name = new std::string(enemy_name);
			action->pos = new sf::Vector2f(pos_x, pos_y);
			action->state = new size_t(state);
			action->facing = new size_t(facing);

			action_stream << action;
		}
	}

	map_ground.setTexture(game->assets->getTexture(texture_name));
	map_ground.loadLevel(tile_size, level_layer, map_size);
	delete level_layer;

	while (!action_stream.empty()) {
		Action* action;
		action_stream >> action;
		doAction(action);
	}
}

void ScenePlay::update() {
	if (!paused) {
		ent_mgr.update();
		//sEnemySpawner();
		sLevelUp();
		sLifespan();
		sAI();
		//sMissleGuidance();
		SUpdate::updatePosition(ent_mgr.getEntities(), map_ground.getBounds());
		sCollisionCheck();
		sCollisionSolve();
		sState();
		sFireWeapon();
		sInterface();
	}

	sAnimation();
	sView();

	game->screen_tex.draw(map_ground);

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
}

void ScenePlay::spawnEnemy() {
	sf::Vector2f dir(rand()%2, rand()%2);
	if (rand()%2 == 0) dir.x *= -1;
	if (rand()%2 == 0) dir.y *= -1;

	bool position_is_valid = false;
	sf::Vector2f pos;
	// deprecated
	int player_radius = 50;

	std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_ENEMY);
	// deprecated
	int radius = 50;

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
	e->get<CStats>()->level = rand() % 10;
}

void ScenePlay::spawnEntity(size_t tag, std::string& recipe_name, sf::Vector2f& pos, size_t state, size_t facing) {
	sf::Vector2f dir(0, 0);

	switch (facing) {
		case Entity::FACING_E: dir = {1, 0}; break;
		case Entity::FACING_NE: dir = {1, -1}; break;
		case Entity::FACING_N: dir = {0, -1}; break;
		case Entity::FACING_NW: dir = {-1, -1}; break;
		case Entity::FACING_W: dir = {-1, 0}; break;
		case Entity::FACING_SW: dir = {-1, 1}; break;
		case Entity::FACING_S: dir = {0, 1}; break;
		case Entity::FACING_SE: dir = {1, 1}; break;
	}

	std::shared_ptr<Entity> e = ent_mgr.add(tag, recipe_name);

	e->state = state;
	e->facing = facing;
	e->get<CTransform>()->pos = pos;
	e->get<CTransform>()->dir = dir;
	e->get<CTransform>()->prev_dir = dir;
	e->get<CAnimation>()->active_anim = &e->get<CAnimation>()->anim_set.animations[state][facing];
}

void ScenePlay::sEnemySpawner() {
	if (frame_current % 100 == 0) {
		spawnEnemy();
	}
}

bool ScenePlay::checkCollision(std::shared_ptr<Entity>& a, std::shared_ptr<Entity>& b) {
	bool collision = false;

	if (a->get<CCollision>() && b->get<CCollision>()) {
		if (!a->get<CCollision>()->hitbox.empty() && !b->get<CCollision>()->hitbox.empty()) {
			for (HitBox& hb_a : a->get<CCollision>()->hitbox) {
				for (HitBox& hb_b : b->get<CCollision>()->hitbox) {
					sf::Vector2f pos_a = a->get<CTransform>()->pos + hb_a.offset[a->facing];
					sf::Vector2f pos_b = b->get<CTransform>()->pos + hb_b.offset[b->facing];

					float square_distance = squareDistance(pos_a, pos_b);
					int square_radius = (hb_a.radius + hb_b.radius) * (hb_a.radius + hb_b.radius);

					if (square_distance < square_radius) {
						collision = true;

						a->get<CCollision>()->colliders.push_back(b);
						a->get<CCollision>()->hitboxes_this.push_back(&hb_a);
						a->get<CCollision>()->hitboxes_collider.push_back(&hb_b);

						b->get<CCollision>()->colliders.push_back(a);
						b->get<CCollision>()->hitboxes_this.push_back(&hb_b);
						b->get<CCollision>()->hitboxes_collider.push_back(&hb_a);
					}
				}
			}
		}
	}

	return collision;
}

void ScenePlay::sCollisionCheck() {
	EntityVec& entities = ent_mgr.getEntities();

	// clear colliders
	for (size_t i=0; i<entities.size(); i++) {
		if (entities[i]->get<CCollision>()) {
			entities[i]->get<CCollision>()->colliders.clear();
			entities[i]->get<CCollision>()->hitboxes_this.clear();
			entities[i]->get<CCollision>()->hitboxes_collider.clear();
		}
	}

	for (size_t i=0; i<entities.size(); i++) {
		for (size_t j=i+1; j<entities.size(); j++) {
			checkCollision(entities[i], entities[j]);
		}
	}
}

void ScenePlay::sCollisionSolve() {
	for (std::shared_ptr<Entity>& entity : ent_mgr.getEntities()) {

		// if it's not a projectile and has CCollision
		if (!entity->get<CLifespan>() && entity->get<CCollision>() && entity->alive) {
			EntityVec& colliders = entity->get<CCollision>()->colliders;
			std::vector<HitBox*>& hitboxes_this = entity->get<CCollision>()->hitboxes_this;
			std::vector<HitBox*>& hitboxes_collider = entity->get<CCollision>()->hitboxes_collider;

			// if collided with something
			if (colliders.size() > 0) {
				for (size_t i = 0; i < colliders.size(); i++) {

					// if it's a colliders[i](projectile) apply damage and kill colliders[i]
					if (colliders[i]->get<CLifespan>() && colliders[i]->alive) {
						colliders[i]->alive = false;
						spawnExplosion(colliders[i]->get<CTransform>()->pos);

						if (entity->get<CStats>() && colliders[i]->get<CStats>()) {

							const int& collider_atk = colliders[i]->get<CStats>()->effective[CStats::ATTACK];
							int& entity_hp = entity->get<CStats>()->effective[CStats::HEALTH];
							int& entity_def = entity->get<CStats>()->effective[CStats::DEFENCE];

							if (entity_def > 0) {
								entity_def -= collider_atk;

								if (entity_def < 0) {
									entity_hp -= -entity_def;
									entity_def = 0;
								}
							}
							else {
								entity_hp -= collider_atk;
							}

							if (entity_hp <= 0) {
								entity->alive = false;

								if (entity->get<CScore>()) {
									score += entity->get<CScore>()->score;
								}
							}
						}
					}

					// else if it's not a collider(projectile) move to previous position
					else {
						// if the entity is able to move and the collider it's not a projectile;
						entity->get<CTransform>()->pos = entity->get<CTransform>()->prev_pos;

						if (entity->get<CTransform>()->max_velocity && !colliders[i]->get<CLifespan>()) {

							const sf::Vector2f& e_pos = entity->get<CTransform>()->pos + hitboxes_this[i]->offset[entity->facing];
							const sf::Vector2f& c_pos = colliders[i]->get<CTransform>()->pos + hitboxes_collider[i]->offset[colliders[i]->facing];
							const sf::Vector2f delta = e_pos - c_pos;

							float dx = 0;
							float dy = 0;

							float velocity = entity->get<CTransform>()->max_velocity;

							if (delta.y > 0) dy += velocity;
							else if (delta.y < 0) dy -= velocity;

							if (delta.x > 0) dx += velocity;
							else if (delta.x < 0) dx -= velocity;

							switch (entity->facing) {
								case Entity::FACING_E:
								case Entity::FACING_W:
									dx = 0;
								break;
								case Entity::FACING_N:
								case Entity::FACING_S:
									dy = 0;
								break;
								case Entity::FACING_NE:
								case Entity::FACING_NW:
								case Entity::FACING_SE:
								case Entity::FACING_SW:
									if (abs(delta.x) > abs(delta.y))
										dy = 0;
									else
										dx = 0;
								break;
							}

							 entity->get<CTransform>()->pos.x += dx;
							 entity->get<CTransform>()->pos.y += dy;

						}
					}
				}
			}
		}
	}
}

void ScenePlay::sState() {
	size_t state_new = Entity::STATE_IDLE;

	for (std::shared_ptr<Entity>& entity : ent_mgr.getEntities()) {
		if (entity->get<CTransform>()) {
			if (entity->get<CTransform>()->vel.x || entity->get<CTransform>()->vel.y) {
				state_new = Entity::STATE_RUN;
			}
			else if (entity->state != Entity::STATE_DIE) {
				state_new = Entity::STATE_IDLE;
			}
		}


		if (!entity->alive) {
			state_new = Entity::STATE_DIE;
		}

		entity->state = state_new;
	}
}

void ScenePlay::spawnExplosion(sf::Vector2f& pos) {
	std::string recipe = "glowing_bullet_explosion";
	std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_SFX, recipe);

	e->add<CTransform>(new CTransform(pos, 0));

	e->state = Entity::STATE_DIE;
	e->alive = true;
}

void ScenePlay::sAI() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities(Entity::TAG_ENEMY)) {
		if (e->get<CWeapon>() && e->get<CInput>()) {
			e->get<CInput>()->fire_primary = true;
		}
	}
}

void ScenePlay::sFireWeapon() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities()) {
		if (e->get<CWeapon>()) {
			CWeapon& comp_w = *e->get<CWeapon>();
			sf::Vector2f pos(e->get<CTransform>()->pos + e->get<CWeapon>()->projectile_spawn[e->facing]);

			if (comp_w.p_cooldown_current > 0) {
				comp_w.p_cooldown_current--;
				e->get<CInput>()->fire_primary = false;
			}

			if (comp_w.s_cooldown_current > 0) {
				comp_w.s_cooldown_current--;
				e->get<CInput>()->fire_secondary = false;
			}

			if (e->get<CInput>()->fire_primary && comp_w.p_cooldown_current == 0) {
				spawnEntity(comp_w.p_tag, comp_w.primary, pos, Entity::STATE_RUN, e->facing);

				e->get<CInput>()->fire_primary = false;
				comp_w.p_cooldown_current = comp_w.p_cooldown;
			}
			else if (e->get<CInput>()->fire_secondary && comp_w.s_cooldown_current == 0) {
				spawnEntity(comp_w.s_tag, comp_w.secondary, pos, Entity::STATE_RUN, e->facing);

				e->get<CInput>()->fire_secondary = false;
				comp_w.s_cooldown_current = comp_w.s_cooldown;
			}
		}
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

		if (remaining <= 0) {
			e->alive = false;
		}
	}
}

void ScenePlay::killEntity(std::shared_ptr<Entity>& entity) {

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
				spawnEntity(*a->ent_tag, *a->ent_name, *a->pos, *a->state, *a->facing);
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

			const size_t state = e->state;
			AnimMapState& animations = e->get<CAnimation>()->anim_set.animations;
			Animation*& active_anim = e->get<CAnimation>()->active_anim;
			size_t has_state_animation = animations.count(state);

			active_anim->update();

			if (active_anim->hasEnded() || active_anim->play == Animation::PLAY_LOOP) {
				if (e->get<CTransform>()) {
					if (has_state_animation != 0) {
						sf::Vector2f e_dir(e->get<CTransform>()->prev_dir);
						size_t facing(1);

						if (e_dir.x == 1 && e_dir.y == 0) facing = Entity::FACING_E;
						if (e_dir.x == 1 && e_dir.y == -1) facing = Entity::FACING_NE;
						if (e_dir.x == 0 && e_dir.y == -1) facing = Entity::FACING_N;
						if (e_dir.x == -1 && e_dir.y == -1) facing = Entity::FACING_NW;
						if (e_dir.x == -1 && e_dir.y == 0) facing = Entity::FACING_W;
						if (e_dir.x == -1 && e_dir.y == 1) facing = Entity::FACING_SW;
						if (e_dir.x == 0 && e_dir.y == 1) facing = Entity::FACING_S;
						if (e_dir.x == 1 && e_dir.y == 1) facing = Entity::FACING_SE;

						if (animations[state].count(facing) == 0) {
							facing = animations[state].begin()->first;
						}

						e->facing = facing;

						if (active_anim != &animations[state][facing]) {
							active_anim = &animations[state][facing];
						}
					}
				}
			}

			// update animation sprite position
			if (e->get<CTransform>()) {
				active_anim->getSprite().setPosition(e->get<CTransform>()->pos);
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
	sf::FloatRect world = map_ground.getBounds();
	sf::FloatRect rect(cam.pos.x-w/2, cam.pos.y-h/2, w, h);

	//fix weird lines between map tiles when moving
	rect.left = round(rect.left);
	rect.top = round(rect.top);

	if (rect.left < 0) rect.left = 0;
	if (rect.top < 0) rect.top = 0;
	if (rect.left + rect.width > world.width) rect.left = world.width - w;
	if (rect.top + rect.height > world.height) rect.top = world.height - h;

	game->view.reset(rect);
}

float ScenePlay::squareDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}
