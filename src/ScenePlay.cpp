#include "ScenePlay.h"
#include <cmath>
#include "SUpdate.h"
#include "SDraw.h"

ScenePlay::ScenePlay(Game* g, std::string lp)
	:Scene(g)
	,level_path(lp)
	,total_kills(0)
	,game_state(GAME_PLAY)
{
	init();
}

ScenePlay::~ScenePlay() {}

void ScenePlay::init() {
	PROFILE_FUNCTION();

	setFade(FADE_IN, 60);

	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::A, Action::MOVE_LEFT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::D, Action::MOVE_RIGHT);

	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::N, Action::FIRE_PRIMARY);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::M, Action::FIRE_SECONDARY);

	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::P, Action::GAME_PAUSE);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Escape, Action::CHANGE_SCENE_MENU);

	load_level(level_path);

	static_cast<WidgetText*>(game->assets->getWidget("player_health_text"))->linkToInt(player->get<CStats>()->effective[CStats::HEALTH]);
	static_cast<WidgetText*>(game->assets->getWidget("base_health_text"))->linkToInt(base->get<CStats>()->effective[CStats::HEALTH]);
	static_cast<WidgetText*>(game->assets->getWidget("total_kills_text"))->linkToInt(total_kills);

	interface.add(game->assets->getWidget("player_health"));
	interface.add(game->assets->getWidget("base_health"));
	interface.add(game->assets->getWidget("total_kills"));

	// run this block to display level;
	{
		ent_mgr.update();
		SUpdate::updatePosition(ent_mgr.getEntities(), map_ground.getBounds());
		sCollisionCheck();
		sCollisionSolve();
		sStateFacing();
		sFireWeapon();
		sInterface();
		sAnimation();
		// focus camera on player
		cam.pos = player->get<CTransform>()->pos;
	}
}

void ScenePlay::load_level(std::string path) {
	PROFILE_FUNCTION();

	std::ifstream file(path);
	std::string word;

	sf::Vector2u tile_size(0, 0), map_size(0, 0);
	std::string texture_name = "";
	int* level_layer;

	int frame, pos_x, pos_y, dir_x, dir_y;
	size_t enemy_name;
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
					if (word == "spawn") code = Action::SPAWN_ENTITY;
					else if (word  == "spawn_player") code = Action::SPAWN_PLAYER;
					else if (word  == "spawn_base") code = Action::SPAWN_BASE;
				}
				else if (word == "type") {
					file >> word;
					if (word == "start") type = Action::TYPE_START;
					else if (word == "end") type = Action::TYPE_END;
				}
				else if (word == "frame") file >> frame;
				else if (word == "entity") {
					file >> word;
					if (word == "enemy") tag = Entity::TAG_ENEMY;
					else if (word == "player") tag = Entity::TAG_PLAYER;
					else if (word == "base") tag = Entity::TAG_BASE;
					else if (word == "environment") tag = Entity::TAG_ENVIRONMENT;
					else {
						std::cout << "LOAD LEVEL: Tag \"" << "\" is not supported\n";
						exit(0);
					}

					file >> word;
					enemy_name = game->assets->getRecipeNameID(word);
				}
				else if (word == "pos") file >> pos_x >> pos_y;
				else if (word == "state") {
					file >> word;
					if (word == "idle") state = Entity::STATE_IDLE;
					else if (word == "run") state = Entity::STATE_RUN;
					else if (word == "die") state = Entity::STATE_DIE;
					else if (word == "spawn") state = Entity::STATE_SPAWN;
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
			action->ent_name = new size_t(enemy_name);
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
	{
		PROFILE_SCOPE("SCENE_LOGIC");

		if (!paused && game_state == GAME_PLAY && !isFading()) {
			ent_mgr.update();

			//sEnemySpawner();
			sLevelUp();
			sLifespan();
			sAI();
			//sMissleGuidance();

			SUpdate::updatePosition(ent_mgr.getEntities(), map_ground.getBounds());

			sCollisionCheck();
			sCollisionSolve();
			sStateFacing();
			sFireWeapon();
			sInterface();
			sAnimation();
			sGameState();
		}
		sView();
	}

	{
		PROFILE_SCOPE("sDrawBg");
		game->screen_tex.draw(map_ground);
	}

	{
		PROFILE_SCOPE("sDrawEntities");
		SDraw::drawEntities(&game->screen_tex, ent_mgr.getEntities());
	}

	if (glitter.m_lifetime >=0) {
		glitter.update();
		game->screen_tex.draw(glitter);
	}

	//change view in order to keep the interface relative to window
	{
		PROFILE_SCOPE("sDrawInterface");
		game->screen_tex.setView(gui_view);
		SDraw::drawInterface(&game->screen_tex, interface.getWidgets());
		game->screen_tex.setView(game->view);
	}

	sFade();
	frame_current++;
}


void ScenePlay::spawnPlayer() {
	const sf::Vector2f pos(game->app_conf.window_w/2, game->app_conf.window_h/2);

	player = ent_mgr.add(Entity::TAG_PLAYER);
	player->get<CTransform>()->pos = pos;
}

void ScenePlay::spawnBase() {
	const sf::Vector2f pos(game->app_conf.window_w/2 + 200, game->app_conf.window_h/2);


	size_t recipe = game->assets->getRecipeNameID("cow");
	base = ent_mgr.add(Entity::TAG_ENEMY, recipe);
	base->get<CTransform>()->pos = pos;
	base->get<CTransform>()->dir = {-1, 1};
	base->state = Entity::STATE_IDLE;
	base->facing = Entity::FACING_SW;
	base->get<CTransform>()->prev_dir = {-1, 1};
	base->get<CAnimation>()->active_anim = &base->get<CAnimation>()->anim_set.animations[base->state][base->facing];
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

void ScenePlay::spawnEntity(size_t tag, size_t recipe_name, sf::Vector2f& pos, size_t state, size_t facing) {
	spawnEntity(tag, recipe_name, nullptr, pos, state, facing);
}

void ScenePlay::spawnEntity(size_t tag, size_t recipe_name, std::shared_ptr<Entity> owner, sf::Vector2f& pos, size_t state, size_t facing) {

#ifdef DEBUG_SPAWN_ENTITY
	std::cout << "spawnEntity()  tag: " << tag << " name_id: " << recipe_name << " state: " << state << " facing: " << facing << std::endl;
#endif

	sf::Vector2f dir = dirOf(facing);

	std::shared_ptr<Entity> e = ent_mgr.add(tag, recipe_name);

	if (e) {
		if (state == Entity::STATE_SPAWN) e->blocked = true;
		e->owner = owner;
		e->state = state;
		e->facing = facing;

		e->get<CTransform>()->pos = pos;
		e->get<CTransform>()->dir = dir;
		e->get<CTransform>()->prev_dir = dir;
		e->get<CAnimation>()->active_anim = &e->get<CAnimation>()->anim_set.animations[state][facing];

		if (e->get<CBPatrol>()) {
			e->get<CBPatrol>()->base_pos = pos;
		}
	}
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
	PROFILE_FUNCTION();

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
	PROFILE_FUNCTION();

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

						// hit flag is set only for one frame
						// it will be unset by sStateFacing
						entity->hit = true;

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

								if (colliders[i]->owner) {
									if (colliders[i]->owner == player && entity->tag == Entity::TAG_ENEMY) {
										kills_per_enemy[entity->name]++;
										total_kills++;
									}
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

void ScenePlay::sStateFacing() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities()) {

		// update facing direction;
		if (!e->blocked) {
			sf::Vector2f dir(e->get<CTransform>()->prev_dir);
			if (dir.x == 1 && dir.y == 0) e->facing = Entity::FACING_E;
			if (dir.x == 1 && dir.y == -1) e->facing = Entity::FACING_NE;
			if (dir.x == 0 && dir.y == -1) e->facing = Entity::FACING_N;
			if (dir.x == -1 && dir.y == -1) e->facing = Entity::FACING_NW;
			if (dir.x == -1 && dir.y == 0) e->facing = Entity::FACING_W;
			if (dir.x == -1 && dir.y == 1) e->facing = Entity::FACING_SW;
			if (dir.x == 0 && dir.y == 1) e->facing = Entity::FACING_S;
			if (dir.x == 1 && dir.y == 1) e->facing = Entity::FACING_SE;
		}

		//change state;
		if (!e->blocked) {
			if (e->get<CTransform>()) {
				if (e->get<CTransform>()->vel.x || e->get<CTransform>()->vel.y) {
					e->state = Entity::STATE_RUN;
				}
				else {
					e->state = Entity::STATE_IDLE;
				}
			}

			if (e->hit && e->get<CAnimation>()->anim_set.animations[Entity::STATE_HIT].count(e->facing) != 0) {
				e->state = Entity::STATE_HIT;
				e->blocked = true;
				e->hit = false;
			}

			// block when firing only if it has firing animation
			if (e->get<CInput>() && e->get<CWeapon>()) {
				CWeapon& comp_w = *e->get<CWeapon>();

				if (e->get<CInput>()->fire_primary && comp_w.p_cooldown_current == 0 && comp_w.p_rounds) {
					if (e->get<CAnimation>()->anim_set.animations[Entity::STATE_FIRE_PRIMARY].count(e->facing) != 0) {
						e->state = Entity::STATE_FIRE_PRIMARY;
						e->blocked = true;
					}
				}
				else if (e->get<CInput>()->fire_secondary && comp_w.s_cooldown_current == 0 && comp_w.s_rounds) {
					if (e->get<CAnimation>()->anim_set.animations[Entity::STATE_FIRE_SECONDARY].count(e->facing) != 0) {
						e->state = Entity::STATE_FIRE_SECONDARY;
						e->blocked = true;
					}
				}
			}
		}
		//if blocking animation has ended
		else if (e->get<CAnimation>()->active_anim->hasEnded()) {
			e->blocked = false;
			e->state = Entity::STATE_IDLE;
		}

		if (!e->alive) {
			if (e->blocked && e->state != Entity::STATE_DIE) {
				e->get<CAnimation>()->active_anim->has_ended = true;
			}

			e->state = Entity::STATE_DIE;
		}
	}
}

void ScenePlay::spawnExplosion(sf::Vector2f& pos) {
	size_t recipe = game->assets->getRecipeNameID("glowing_bullet_explosion");
	std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_SFX, recipe);

	e->add<CTransform>(new CTransform(pos, 0));

	e->state = Entity::STATE_DIE;
	e->alive = true;
}

void ScenePlay::sAI() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities()) {
		bool has_target = false;

		if (e->get<CBFire>() && e->get<CWeapon>() && e->get<CInput>()) {
			if (e->get<CWeapon>()->primary) {
				handle(e, e->get<CBFire>()->pri, e->get<CInput>()->fire_primary);
			}
			if (e->get<CWeapon>()->secondary) {
				handle(e, e->get<CBFire>()->sec, e->get<CInput>()->fire_secondary);
			}

			if (e->get<CBFire>()->target && e->tag != Entity::TAG_ENVIRONMENT) {
				has_target = true;

				// face target
				size_t facing = facingOf(e->get<CBFire>()->target->get<CTransform>()->pos - e->get<CTransform>()->pos);
				sf::Vector2f dir = dirOf(facing);

				// reset all directions
				e->get<CInput>()->right = false;
				e->get<CInput>()->left = false;
				e->get<CInput>()->up = false;
				e->get<CInput>()->down = false;

				if (dir.x > 0) e->get<CInput>()->right = true;
				else if (dir.x < 0) e->get<CInput>()->left = true;
				if (dir.y > 0) e->get<CInput>()->down = true;
				else if (dir.y < 0) e->get<CInput>()->up = true;
			}
		}

		if (e->get<CBPatrol>() && e->get<CInput>() && !has_target) {
			switch (e->get<CBPatrol>()->patrol) {
				case CBPatrol::PATROL_HORIZONTAL:
					if (e->get<CTransform>()->pos.x - e->get<CBPatrol>()->base_pos.x <= 0) {
						e->get<CInput>()->right = true;
						e->get<CInput>()->left = false;
					}
					else if (e->get<CTransform>()->pos.x - e->get<CBPatrol>()->base_pos.x >= e->get<CBPatrol>()->dist) {
						e->get<CInput>()->right = false;
						e->get<CInput>()->left = true;
					}

					// if it has lost y position
					if (e->get<CTransform>()->pos.y - e->get<CBPatrol>()->base_pos.y >= 30) {
						e->get<CInput>()->up = true;
						e->get<CInput>()->down = false;
					}
					else if (e->get<CTransform>()->pos.y - e->get<CBPatrol>()->base_pos.y <= -30) {
						e->get<CInput>()->up = false;
						e->get<CInput>()->down = true;
					}
					else {
						e->get<CInput>()->up = false;
						e->get<CInput>()->down = false;
					}
				break;

				case CBPatrol::PATROL_VERTICAL:
					if (e->get<CTransform>()->pos.y - e->get<CBPatrol>()->base_pos.y <= 0) {
						e->get<CInput>()->up = false;
						e->get<CInput>()->down = true;
					}
					else if (e->get<CTransform>()->pos.y - e->get<CBPatrol>()->base_pos.y >= e->get<CBPatrol>()->dist) {
						e->get<CInput>()->up = true;
						e->get<CInput>()->down = false;
					}

					// if it has lost x position
					if (e->get<CTransform>()->pos.x - e->get<CBPatrol>()->base_pos.x >= 30) {
						e->get<CInput>()->left = true;
						e->get<CInput>()->right = false;
					}
					else if (e->get<CTransform>()->pos.x - e->get<CBPatrol>()->base_pos.x <= -30) {
						e->get<CInput>()->left = false;
						e->get<CInput>()->right = true;
					}
					else {
						e->get<CInput>()->left = false;
						e->get<CInput>()->right = false;
					}
				break;
			}
		}
	}
}

void ScenePlay::handle(std::shared_ptr<Entity>& e, const BFire& b_fire, bool& fire_weapon) {
	switch (b_fire.trigger) {
		case CBFire::TR_CONTINUOUS:
			fire_weapon = true;
		break;
		case CBFire::TR_RANDOM:
			if (rand() % 2) fire_weapon = true;
			else fire_weapon = false;
		break;
		case CBFire::TR_PLAYER_NEARBY:
		{
			float sq_dist = squareDistance(e->get<CTransform>()->pos, player->get<CTransform>()->pos);
			if (sq_dist < b_fire.data *  b_fire.data) {
				e->get<CBFire>()->target = player;
				fire_weapon = true;
			}
			// lose agro if target got too far
			else if (sq_dist > b_fire.data * b_fire.data * 4) {
				e->get<CBFire>()->target = nullptr;
			}
		}
		break;
		case CBFire::TR_BASE_NEARBY:
			if (squareDistance(e->get<CTransform>()->pos, base->get<CTransform>()->pos) < b_fire.data *  b_fire.data) {
				e->get<CBFire>()->target = base;
				fire_weapon = true;
			}
		break;
		case CBFire::TR_BASE_NOT_PROTECTED:
			if (squareDistance(player->get<CTransform>()->pos, base->get<CTransform>()->pos) > b_fire.data *  b_fire.data) {
				e->get<CBFire>()->target = base;
				fire_weapon = true;
			}
		break;
	}
}

void ScenePlay::sFireWeapon() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities()) {
		if (e->get<CWeapon>() && e->alive) {
			CWeapon& comp_w = *e->get<CWeapon>();
			size_t facing(e->facing);

			if (e->get<CBFire>()) {
				if (e->get<CBFire>()->target) {
					facing = facingOf(e->get<CBFire>()->target->get<CTransform>()->pos - e->get<CTransform>()->pos);
				}
			}

			sf::Vector2f pos(e->get<CTransform>()->pos + e->get<CWeapon>()->projectile_spawn[facing]);

			if (comp_w.p_cooldown_current > 0) {
				comp_w.p_cooldown_current--;
				e->get<CInput>()->fire_primary = false;
			}

			if (comp_w.s_cooldown_current > 0) {
				comp_w.s_cooldown_current--;
				e->get<CInput>()->fire_secondary = false;
			}

			// use only one weapon at a time
			// the weapon cooldown time should be slightly higher than the firing animation
			if (comp_w.p_cooldown_current == 0 && comp_w.s_cooldown_current == 0) {
				if (e->get<CInput>()->fire_primary && comp_w.p_rounds) {
					spawnEntity(comp_w.p_tag, comp_w.primary, e, pos, Entity::STATE_RUN, facing);

					e->get<CInput>()->fire_primary = false;
					comp_w.p_rounds--;
					comp_w.p_cooldown_current = comp_w.p_cooldown;
				}
				else if (e->get<CInput>()->fire_secondary && comp_w.s_rounds) {
					spawnEntity(comp_w.s_tag, comp_w.secondary, e, pos, Entity::STATE_RUN, facing);

					e->get<CInput>()->fire_secondary = false;
					comp_w.s_rounds--;
					comp_w.s_cooldown_current = comp_w.s_cooldown;
				}
			}
		}
	}
}

void ScenePlay::sLifespan() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities(Entity::TAG_PROJECTILE)) {
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

float ScenePlay::angle(sf::Vector2f v) {
	v.y*=-1;
	float ang = atan(v.y/v.x);

	if (v.x < 0 && v.y > 0) ang += PI;
	else if (v.x < 0 && v.y < 0) ang += PI;
	else if (v.x > 0 && v.y < 0) ang += 2*PI;

	return ang / PI * 180;
}

size_t ScenePlay::facingOf(sf::Vector2f v) {
	float ang = angle(v);

	if (ang >= 0 && ang < 22.5) return Entity::FACING_E;
	else if (ang >= 337.5 && ang < 360) return Entity::FACING_E;
	else if (ang >= 22.5 && ang < 67.5) return Entity::FACING_NE;
	else if (ang >= 67.5 && ang < 112.5) return Entity::FACING_N;
	else if (ang >= 112.5 && ang < 157.5) return Entity::FACING_NW;
	else if (ang >= 112.5 && ang < 202.5) return Entity::FACING_W;
	else if (ang >= 202.5 && ang < 247.5) return Entity::FACING_SW;
	else if (ang >= 247.5 && ang < 292.5) return Entity::FACING_S;
	else if (ang >= 292.5 && ang < 337.5) return Entity::FACING_SE;
}

sf::Vector2f ScenePlay::dirOf(size_t facing) {
	switch (facing) {
		case Entity::FACING_E: return {1, 0}; break;
		case Entity::FACING_NE: return {1, -1}; break;
		case Entity::FACING_N: return {0, -1}; break;
		case Entity::FACING_NW: return {-1, -1}; break;
		case Entity::FACING_W: return {-1, 0}; break;
		case Entity::FACING_SW: return {-1, 1}; break;
		case Entity::FACING_S: return {0, 1}; break;
		case Entity::FACING_SE: return {1, 1}; break;
	}
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
			case Action::CHANGE_SCENE_MENU:
				setFade(FADE_OUT, 60, Game::GAME_SCENE_MENU);
			break;
			case Action::SPAWN_ENTITY:
				spawnEntity(*a->ent_tag, *a->ent_name, *a->pos, *a->state, *a->facing);
			break;
			case Action::SPAWN_PLAYER:
				spawnEntity(*a->ent_tag, *a->ent_name, *a->pos, *a->state, *a->facing);
				ent_mgr.update();
				player = ent_mgr.getEntities(Entity::TAG_PLAYER)[0];
			break;
			case Action::SPAWN_BASE:
				spawnEntity(*a->ent_tag, *a->ent_name, *a->pos, *a->state, *a->facing);
				ent_mgr.update();
				base = ent_mgr.getEntities(Entity::TAG_BASE)[0];
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
	static_cast<WidgetText*>(game->assets->getWidget("player_health_text"))->updateText();
	static_cast<WidgetText*>(game->assets->getWidget("base_health_text"))->updateText();
	static_cast<WidgetText*>(game->assets->getWidget("total_kills_text"))->updateText();
}

void ScenePlay::sAnimation() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities()) {
		if (e->get<CAnimation>() && e->facing != 0) {

			const size_t state = e->state;
			size_t& facing = e->facing;
			AnimMapState& animations = e->get<CAnimation>()->anim_set.animations;
			Animation*& active_anim = e->get<CAnimation>()->active_anim;
			size_t has_state_animation = animations.count(state);

			active_anim->update();

			if ((active_anim->hasEnded() || active_anim->play == Animation::PLAY_LOOP)) {
				if (has_state_animation != 0) {
					if (animations[state].count(facing) == 0) {
						facing = animations[state].begin()->first;
					}

					if (active_anim != &animations[state][facing]) {
						// set new animation
						active_anim = &animations[state][facing];
						active_anim->reset();
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

void ScenePlay::sGameState() {
	if (!player->alive && player->get<CAnimation>()->active_anim->hasEnded() ||
		!base->alive && base->get<CAnimation>()->active_anim->hasEnded())
	{
		game_state = GAME_OVER;
		game->addKills(kills_per_enemy);
		game->stageReset();
		setFade(FADE_OUT, 60, Game::GAME_SCENE_OVER);
	}

	if (ent_mgr.getEntities(Entity::TAG_ENEMY).empty()) {
		game_state = GAME_OVER;
		game->addKills(kills_per_enemy);
		if (game->stageNext()) {
			setFade(FADE_OUT, 60, Game::GAME_SCENE_SCORE);
		}
		else {
			game->stageReset();
			setFade(FADE_OUT, 60, Game::GAME_SCENE_OVER);
		}
	}
#ifdef DEBUG_ENEMIES_LEFT
	else {
		std::cout << "enemies left: " << ent_mgr.getEntities(Entity::TAG_ENEMY).size() << std::endl;
	}
#endif
}

float ScenePlay::squareDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}
