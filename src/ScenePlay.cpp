#include "ScenePlay.h"
#include <cmath>
#include "SUpdate.h"
#include "SDraw.h"
#include "SharedResources.h"
#include "Profiler.h"

ScenePlay::ScenePlay(std::string lp)
	:Scene(GAME_SCENE::PLAY)
	,level_path(lp)
	,total_kills(0)
	,collision_map(ent_mgr)
{
	init();
}

ScenePlay::ScenePlay(size_t t, std::string lp)
	:Scene(t)
	,level_path(lp)
	,total_kills(0)
	,collision_map(ent_mgr)
{}

ScenePlay::~ScenePlay() {}

void ScenePlay::init() {

	game_stats->state = GameStats::State::PLAY;
	PROFILE_FUNCTION();

	music_fade_out = true;

	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::A, Action::MOVE_LEFT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::D, Action::MOVE_RIGHT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::N, Action::FIRE_PRIMARY);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::M, Action::FIRE_SECONDARY);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::P, Action::GAME_PAUSE);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Escape, Action::CHANGE_SCENE_MENU);

	load_level(level_path);
	collision_map.setMap(level.map_size, level.tile_size, app_conf->colmap_res, app_conf->colmap_update);

	// setup interface
	interface.add(assets->getWidget("play_ui"));
	interface.add(assets->getWidget("blood_overlay"));

	std::variant<int*, std::string*> links[static_cast<int>(Link::Target::COUNT)];
	links[static_cast<int>(Link::Target::PLAYER_HP)] = &player->get<CStats>()->effective[CStats::HEALTH];
	links[static_cast<int>(Link::Target::BASE_HP)] = &base->get<CStats>()->effective[CStats::HEALTH];
	links[static_cast<int>(Link::Target::TOTAL_KILLS)] = &total_kills;
	links[static_cast<int>(Link::Target::SECONDARY_ROUNDS)] = &player->get<CWeapon>()->s_rounds;
	links[static_cast<int>(Link::Target::SECONDARY_ROUNDS_CURRENT)] = &player->get<CWeapon>()->s_rounds_current;
	interface.setLinks(links);

	paused_widget = &assets->getWidget("menu_paused");
	sf::Vector2i pos;
	pos.x = static_cast<int>(app_conf->game_w*0.5);
	pos.y = static_cast<int>(app_conf->game_h*0.5);
	paused_widget->setPosAbs(pos);

	// run this block to display level;
	{
		ent_mgr.update();
		SUpdate::updatePosition(ent_mgr.getEntities(), level.map_ground.getBounds());
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
	level = Level(path);
	snd_mgr->playBgMusic(level.bg_music);

	for (Action& a: level.actions) {
		doAction(a);
	}

	ent_mgr.update();
}

void ScenePlay::sPathFind() {
	collision_map.updateColmap();
}

void ScenePlay::drawCollisionLayer() {
	sf::CircleShape circle(level.tile_size.x/(app_conf->colmap_res*2));

	//DRAW BLOCKING
	if (!collision_map.colmap.empty()) {
		circle.setFillColor(sf::Color(255, 0, 0, 50));

		for (int y = 0; y < collision_map.map_size.x; y++) {
			for (int x = 0; x < collision_map.map_size.x; x++) {
				circle.setPosition(sf::Vector2f(x*level.tile_size.x/app_conf->colmap_res,y*level.tile_size.y/app_conf->colmap_res));

				if (collision_map.colmap[x][y]) {
					screen_tex->draw(circle);
				}
			}
		}
	}

	//DRAW PATHS
	circle.setFillColor(sf::Color(0, 255, 255, 80));

	for (std::shared_ptr<Entity>& e: ent_mgr.getEntities()) {
		if (e->get<CBChase>()) {
			if (!e->get<CBChase>()->path.empty()) {
				for (sf::Vector2f p: e->get<CBChase>()->path) {
					circle.setPosition(sf::Vector2f(p.x, p.y));
					screen_tex->draw(circle);
				}
			}
		}
	}
}

void ScenePlay::drawDirectionVectors() {
	for (std::shared_ptr<Entity>& e: ent_mgr.getEntities()) {
		if (e->get<CTransform>()) {
			if (e->get<CTransform>()->max_velocity) {
				sf::Vector2f dir = dirOf(e->facing);
				dir.x *= level.tile_size.x/2;
				dir.y *= level.tile_size.x/2;

				sf::Vertex line[] = {
					sf::Vertex(e->get<CTransform>()->pos),
					sf::Vertex(e->get<CTransform>()->pos + dir)
				};
				screen_tex->draw(line, 2, sf::Lines);
			}
		}
	}
}

void ScenePlay::drawGrid() {
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(level.tile_size));
	rect.setFillColor(sf::Color(0,0,0,0));
	rect.setOutlineColor(sf::Color(0,0,0,50));
	rect.setOutlineThickness(-1);

	for (size_t x = 0; x < level.map_size.x; x++) {
		for (size_t y = 0; y < level.map_size.y; y++) {
			rect.setPosition(sf::Vector2f(x*level.tile_size.x, y*level.tile_size.y));
			screen_tex->draw(rect);
		}
	}
}

void ScenePlay::drawEntityPosition() {
	sf::Color col(255,0,0);
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities()) {
		if (e->get<CTransform>()) {
			sf::Vector2f pos = e->get<CTransform>()->pos;
			float d = 10;

			sf::Vertex crosshair[] {
				sf::Vertex(pos - sf::Vector2f(d,0), col),
				sf::Vertex(pos + sf::Vector2f(d,0), col),
				sf::Vertex(pos - sf::Vector2f(0,d), col),
				sf::Vertex(pos + sf::Vector2f(0,d), col)
			};

			screen_tex->draw(crosshair, 4, sf::Lines);
		}
	}
}

void ScenePlay::update() {
	{
		PROFILE_SCOPE("SCENE_LOGIC");

		if (!paused && game_stats->state == GameStats::State::PLAY && !isFading()) {
			ent_mgr.update();

			//sEnemySpawner();
			sLevelUp();
			sLifespan();
			sPathFind();
			sAI();
			sPowerup();
			//sMissleGuidance();

			SUpdate::updatePosition(ent_mgr.getEntities(), level.map_ground.getBounds());

			sCollisionCheck();
			sCollisionSolve();
			sWidgetFx();
			sStateFacing();
			sFireWeapon();
			sInterface();
			sAnimation();
			sPlayFx();
			sGameState();
		}
		sView();
	}

	{
		PROFILE_SCOPE("sDrawBg");
		screen_tex->draw(level.map_ground);
	}

	{
		PROFILE_SCOPE("sDrawEntities");
		SDraw::drawEntities(&*screen_tex, ent_mgr.getEntities());
	}

#ifdef DEBUG_GRID
	drawGrid();
#endif

#ifdef DEBUG_COLLISION_LAYER
	drawCollisionLayer();
#endif

#ifdef DEBUG_DIRECTION
	drawDirectionVectors();
#endif

#ifdef DEBUG_ENTITY_POS
	drawEntityPosition();
#endif

	if (glitter.m_lifetime >=0) {
		glitter.update();
		screen_tex->draw(glitter);
	}

	//change view in order to keep the interface relative to window
	{
		PROFILE_SCOPE("sDrawInterface");
		screen_tex->setView(gui_view);
		SDraw::drawInterface(&*screen_tex, interface.getWidgets());
		screen_tex->setView(*game_view);
	}

	frame_current++;
}

void ScenePlay::spawnEnemy() {
	sf::Vector2f dir(rand()%2, rand()%2);
	if (rand()%2 == 0) dir.x *= -1;
	if (rand()%2 == 0) dir.y *= -1;

	bool position_is_valid = false;
	sf::Vector2f pos;
	// deprecated
	int player_radius = 50;

	std::shared_ptr<Entity> e = ent_mgr.add(TAG::ENEMY);
	// deprecated
	int radius = 50;

	while (!position_is_valid) {
		pos.x = rand() % static_cast<int>(app_conf->modes[app_conf->current_mode_id].width - radius*2) + radius;
		pos.y = rand() % static_cast<int>(app_conf->modes[app_conf->current_mode_id].height - radius*2) + radius;

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

		if (e->get<CSfx>()) {
			if (e->get<CSfx>()->spawn) {
				snd_mgr->playSoundUnique(e->get<CSfx>()->spawn);
			}
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

bool ScenePlay::checkCollision(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b, const size_t threshold) {
	bool collision = false;

	if (a->get<CCollision>() && b->get<CCollision>()) {
		if (!a->get<CCollision>()->hitbox.empty() && !b->get<CCollision>()->hitbox.empty()) {
			for (HitBox& hb_a : a->get<CCollision>()->hitbox) {
				for (HitBox& hb_b : b->get<CCollision>()->hitbox) {
					sf::Vector2f pos_a = a->get<CTransform>()->pos + hb_a.offset[a->facing];
					sf::Vector2f pos_b = b->get<CTransform>()->pos + hb_b.offset[b->facing];

					float square_distance = squareDistance(pos_a, pos_b);
					int square_radius = (hb_a.radius + hb_b.radius + threshold) * (hb_a.radius + hb_b.radius + threshold);

					if (square_distance < square_radius) {
						collision = true;
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
		if (entity->tag != TAG::PROJECTILE &&
			entity->tag != TAG::POWERUP &&
			entity->tag != TAG::FX &&
			entity->get<CCollision>() && entity->alive)
		{

			EntityVec& colliders = entity->get<CCollision>()->colliders;
			std::vector<HitBox*>& hitboxes_this = entity->get<CCollision>()->hitboxes_this;
			std::vector<HitBox*>& hitboxes_collider = entity->get<CCollision>()->hitboxes_collider;

			// if collided with something
			if (colliders.size() > 0) {
				for (size_t i = 0; i < colliders.size(); i++) {

					// skip if owner spawned the projectile
					if (colliders[i]->owner) {
						if (colliders[i]->owner == entity) {
							continue;
						}
					}

					// skip if collider is POWERUP
					if (colliders[i]->tag == TAG::POWERUP) {
						continue;
					}

					// if the collider[i] is a projectile apply damage and kill colliders[i]
					if (colliders[i]->tag == TAG::PROJECTILE && colliders[i]->alive) {
						colliders[i]->alive = false;
						//spawnExplosion(colliders[i]->get<CTransform>()->pos);

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
								entity_hp = 0;
								entity->alive = false;

								if (colliders[i]->owner) {
									if (colliders[i]->owner == player && entity->tag == TAG::ENEMY) {
										kills_per_enemy[entity->name]++;
										total_kills++;
									}
								}
							}

							if (entity->get<CSfx>()) {
								if (entity_hp <= 0) {
									if (entity->get<CSfx>()->die) {
										snd_mgr->playSoundUnique(entity->get<CSfx>()->die);
									}
								}
								else {
									if (entity->get<CSfx>()->hurt) {
										snd_mgr->playSoundUnique(entity->get<CSfx>()->hurt);
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

void ScenePlay::sPlayFx() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities(TAG::PROJECTILE)) {
		 if (e->get<CAnimation>() && e->get<CFx>()) {
			 for (Fx fx : e->get<CFx>()->fx) {
				 switch (fx.trigger) {
					case TR::DIE:
						if (!e->alive && e->get<CAnimation>()->active_anim->hasEnded()) {
							spawnEntity(fx.tag, fx.id, e->get<CTransform>()->pos, Entity::STATE_DIE, Entity::FACING_E);
						}
					break;
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

				if (e->get<CInput>()->fire_primary && comp_w.p_cooldown_current == 0 && comp_w.p_rounds_current) {
					if (e->get<CAnimation>()->anim_set.animations[Entity::STATE_FIRE_PRIMARY].count(e->facing) != 0) {
						e->state = Entity::STATE_FIRE_PRIMARY;
						e->blocked = true;
					}
				}
				else if (e->get<CInput>()->fire_secondary && comp_w.s_cooldown_current == 0 && comp_w.s_rounds_current) {
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
	size_t recipe = assets->getRecipeNameID("glowing_bullet_explosion");
	std::shared_ptr<Entity> e = ent_mgr.add(TAG::FX, recipe);

	e->add<CTransform>(new CTransform(pos, 0));

	e->state = Entity::STATE_DIE;
	e->alive = true;
}

void ScenePlay::sAI() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities()) {
		bool has_target = false;

		if (e->get<CBChase>()) {
			for (BCondition& bc:e->get<CBChase>()->cond) {
				handleChase(e, bc);
			}

			if (e->get<CBChase>()->target && e->tag != TAG::ENVIRONMENT) {
				std::shared_ptr<Entity>& t = e->get<CBChase>()->target;
				has_target = true;

				sf::Vector2f start = e->get<CTransform>()->pos;
				if (e->get<CCollision>()) {
					if (!e->get<CCollision>()->hitbox.empty()) {
						start += e->get<CCollision>()->hitbox[0].offset[e->facing];
					}
				}

				sf::Vector2f end = t->get<CTransform>()->pos;

				std::vector<sf::Vector2f>& path = e->get<CBChase>()->path;
				bool has_path = collision_map.computePath(start, end, path, MapCollision::MOVE_NORMAL, 0);

				// move twards target if it's not too close;
				size_t range = 0;
				if (e->get<CWeapon>()) {
					size_t p_r = e->get<CWeapon>()->p_range;
					size_t s_r = e->get<CWeapon>()->s_range;

					if (p_r) range = p_r;
					if (s_r && s_r < p_r) range = s_r;
				}

				if (has_path) {
					lookAt(*e->get<CInput>(), path.back(), start);
				}
				else {
					lookAt(*e->get<CInput>(), e->get<CBChase>()->target->get<CTransform>()->pos, e->get<CTransform>()->pos);
				}

				if (checkCollision(e, e->get<CBChase>()->target, range*0.2)) {
					e->get<CInput>()->right = false;
					e->get<CInput>()->left = false;
					e->get<CInput>()->up = false;
					e->get<CInput>()->down = false;
				}
			}
		}

		if (e->get<CBFire>() && e->get<CWeapon>() && e->get<CInput>()) {
			if (e->get<CWeapon>()->primary) {
				for (BCondition& bc: e->get<CBFire>()->pri) {
					handleFire(e, bc, e->get<CInput>()->fire_primary);
				}
			}
			if (e->get<CWeapon>()->secondary) {
				for (BCondition& bc: e->get<CBFire>()->sec) {
					handleFire(e, bc, e->get<CInput>()->fire_secondary);
				}
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

void ScenePlay::sPowerup() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities(TAG::POWERUP)) {
		if (e->get<CBPowerup>()) {
			CBPowerup& cb_powerup = *e->get<CBPowerup>();
			bool cond_met = false;

			switch (cb_powerup.cond.trigger) {
				case TR::PLAYER_NEARBY:
					if (checkCollision(e, player)) {
						cond_met = true;
					}
				break;
			}

			if (cond_met) {
				switch (cb_powerup.powerup) {
					case CBPowerup::PLAYER_HP:
					{
						const int& initial_hp = player->get<CStats>()->initial[CStats::HEALTH];
						int& effective_hp = player->get<CStats>()->effective[CStats::HEALTH];
						int hp_value = cb_powerup.percent * initial_hp / 100;

						effective_hp += hp_value;
						if (effective_hp > initial_hp) effective_hp = initial_hp;
					}
					break;
					case CBPowerup::BASE_HP:
					{
						const int& initial_hp = base->get<CStats>()->initial[CStats::HEALTH];
						int& effective_hp = base->get<CStats>()->effective[CStats::HEALTH];
						int hp_value = cb_powerup.percent * initial_hp / 100;

						effective_hp += hp_value;
						if (effective_hp > initial_hp) effective_hp = initial_hp;
					}
					break;
					case CBPowerup::WEAPON_ROUNDS:
						const int& s_rounds = player->get<CWeapon>()->s_rounds;
						int& s_rounds_current = player->get<CWeapon>()->s_rounds_current;
						int rounds = cb_powerup.percent * s_rounds / 100;

						s_rounds_current += rounds;
						if (s_rounds_current > s_rounds) s_rounds_current = s_rounds;
					break;
				}

				cb_powerup.percent = 0;
				e->alive = false;
			}
		}
	}
}

void ScenePlay::handleChase(std::shared_ptr<Entity>& e, const BCondition& bc) {
	switch (bc.trigger) {
		case TR::PLAYER_NEARBY:
		{
			if (checkCollision(e, player, bc.data_start)) {
				e->get<CBChase>()->target = player;
			}
			// lose agro if target got too far
			else if (!checkCollision(e, player, bc.data_stop)) {
				if (e->get<CBChase>()->target = player) {
					e->get<CBChase>()->target = nullptr;
					e->get<CBChase>()->path.clear();
				}
			}
		}
		break;
		case TR::BASE_NEARBY:
			if (checkCollision(e, base, bc.data_start)) {
				e->get<CBChase>()->target = base;
			}
			// lose agro if target got too far
			else if (!checkCollision(e, base, bc.data_stop)) {
				if (e->get<CBChase>()->target == base) {
					e->get<CBChase>()->target = nullptr;
					e->get<CBChase>()->path.clear();
				}
			}
		break;
		case TR::BASE_NOT_PROTECTED:
			if (!checkCollision(player, base, bc.data_start)) {
				e->get<CBChase>()->target = base;
			}
			// lose agro if target got too far
			else if (checkCollision(player, base, bc.data_stop)) {
				if (e->get<CBChase>()->target == base) {
					e->get<CBChase>()->target = nullptr;
					e->get<CBChase>()->path.clear();
				}
			}
		break;
		case TR::BASE_LOW_HP:
			if (base->get<CStats>()->effective[CStats::HEALTH] < base->get<CStats>()->initial[CStats::HEALTH] * bc.data_start / 100) {
				e->get<CBChase>()->target = base;
			}
		break;
	}
}

void ScenePlay::handleFire(std::shared_ptr<Entity>& e, const BCondition& bc, bool& fire_weapon) {
	// MUST CHECK COLLISION BETWEEN PROJECTILE SPAWN POSTION AND TARGET
	// ADD NEW checkCollision() FUNCTION

	e->get<CBFire>()->target = nullptr;

	switch (bc.trigger) {
		case TR::CONTINUOUS:
			fire_weapon = true;
		break;
		case TR::RANDOM:
			if (rand() % 2) fire_weapon = true;
			else fire_weapon = false;
		break;
		case TR::PLAYER_NEARBY:
			if (checkCollision(e, player, bc.data_start)) {
				fire_weapon = true;
				e->get<CBFire>()->target = player;
			}
		break;
		case TR::BASE_NEARBY:
			if (checkCollision(e, base, bc.data_start)) {
				fire_weapon = true;
				e->get<CBFire>()->target = base;
			}
		break;
	}

	if (e->get<CBChase>()) {
		if (e->get<CBChase>()->target) {
			e->get<CBFire>()->target = e->get<CBChase>()->target;
		}
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
				if (e->get<CInput>()->fire_primary && comp_w.p_rounds_current) {
					if (comp_w.p_delay_current == 0) {
						spawnEntity(comp_w.p_tag, comp_w.primary, e, pos, Entity::STATE_RUN, facing);
						snd_mgr->playSound(comp_w.p_sfx);

						e->get<CInput>()->fire_primary = false;
						comp_w.p_rounds_current--;
						comp_w.p_cooldown_current = comp_w.p_cooldown;
						comp_w.p_delay_current = comp_w.p_delay;
					}
					else {
						comp_w.p_delay_current--;
					}
				}
				else if (e->get<CInput>()->fire_secondary && comp_w.s_rounds_current) {
					if (comp_w.s_delay_current == 0) {
						spawnEntity(comp_w.s_tag, comp_w.secondary, e, pos, Entity::STATE_RUN, facing);
						snd_mgr->playSound(comp_w.s_sfx);

						e->get<CInput>()->fire_secondary = false;
						comp_w.s_rounds_current--;
						comp_w.s_cooldown_current = comp_w.s_cooldown;
						comp_w.s_delay_current = comp_w.s_delay;
					}
					else {
						comp_w.s_delay_current--;
					}
				}
			}
		}
	}
}

void ScenePlay::sLifespan() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities(TAG::PROJECTILE)) {
		checkLifespan(e);
	}

	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities(TAG::FX)) {
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
	sf::Vector2f mouse_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	const sf::Vector2f pos(player->get<CTransform>()->pos);
	const sf::Vector2f dir = mouse_pos - pos;

	std::shared_ptr<Entity> missle = ent_mgr.add(TAG::MISSLE);

	missle->get<CTransform>()->pos = pos;
	missle->get<CTransform>()->dir = dir;
	missle->get<CShape>()->shape.setPosition(pos);
}

std::shared_ptr<Entity> ScenePlay::findTarget(const std::shared_ptr<Entity>& missle) {
	EntityVec reachable;
	sf::Vector2f dir_missle(missle->get<CTransform>()->dir);
	sf::Vector2f dir_enemy;

	for (std::shared_ptr<Entity>& enemy : ent_mgr.getEntities(TAG::ENEMY)) {
		dir_enemy = enemy->get<CTransform>()->pos - missle->get<CTransform>()->pos;

		if (angle(dir_missle, dir_enemy) < 30) {
			reachable.push_back(enemy);
		}
	}

	std::shared_ptr<Entity> target;
	float prev_dist(app_conf->modes[app_conf->current_mode_id].width*app_conf->modes[app_conf->current_mode_id].width);
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
	for (std::shared_ptr<Entity>& missle : ent_mgr.getEntities(TAG::MISSLE)) {

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

	return 0;
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
		default: return {0,0};
	}
}


void ScenePlay::lookAt(CInput& c_input, const sf::Vector2f& a, const sf::Vector2f& b) {
	size_t facing = facingOf(a-b);
	sf::Vector2f dir = dirOf(facing);

	// reset direction
	c_input.right = false;
	c_input.left = false;
	c_input.up = false;
	c_input.down = false;

	// set new direction
	if (dir.x > 0) c_input.right = true;
	else if (dir.x < 0) c_input.left = true;
	if (dir.y > 0) c_input.down = true;
	else if (dir.y < 0) c_input.up = true;
}

void ScenePlay::doAction(const Action& a) {
	if (*a.type == Action::TYPE_START && getCurrentFade() != FADE::OUT) {
		if (*a.code == Action::GAME_PAUSE) {
			paused = !paused;
			if (paused) {
				snd_mgr->pauseBgMusic();
				snd_mgr->playSound("menu_pause");
				interface.add(*paused_widget);
			}
			else {
				snd_mgr->playSound("menu_unpause");
				snd_mgr->playBgMusic();
				interface.getWidgets().pop_back();
			}
		}
		else if (!paused) {
			switch (*a.code) {
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
				case Action::CHANGE_SCENE_MENU:
					setFade(FADE::OUT, GAME_SCENE::MENU);
				break;
				case Action::SPAWN_ENTITY:
					spawnEntity(*a.ent_tag, *a.ent_name, *a.pos, *a.state, *a.facing);
				break;
				case Action::SPAWN_PLAYER:
					spawnEntity(*a.ent_tag, *a.ent_name, *a.pos, *a.state, *a.facing);
					ent_mgr.update();
					player = ent_mgr.getEntities(TAG::PLAYER)[0];
				break;
				case Action::SPAWN_BASE:
					spawnEntity(*a.ent_tag, *a.ent_name, *a.pos, *a.state, *a.facing);
					ent_mgr.update();
					base = ent_mgr.getEntities(TAG::BASE)[0];
				default:
				break;
			}
		}
	}
	if (*a.type == Action::TYPE_END) {
		switch (*a.code) {
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

void ScenePlay::sWidgetFx() {
	WidgetVec& widgets = interface.getWidgets();

	for (Widget& w: widgets) {
		handleWidgetFx(w);
	}
}

void ScenePlay::handleWidgetFx(Widget& w) {
	for (WidgetFx& wfx: w.fx) {
		if (wfx.cond.trigger == TR::PLAYER_HURT && player->hit) {
			w.current_fx = &wfx;
		}
	}

	if (!w.childs.empty()) {
		for (Widget& child: w.getChilds()) {
			handleWidgetFx(child);
		}
	}
}


void ScenePlay::sInterface() {
	interface.update();
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

	if (square_delta > app_conf->cam_treshold) {
		cam.pos += ((cam.target - cam.pos) / app_conf->cam_speed);
	}

	//update view position
	int w = app_conf->game_w;
	int h = app_conf->game_h;
	sf::FloatRect world = level.map_ground.getBounds();
	sf::FloatRect rect(cam.pos.x-w/2, cam.pos.y-h/2, w, h);

	//fix weird lines between map tiles when moving
	rect.left = round(rect.left);
	rect.top = round(rect.top);

	if (rect.left < 0) rect.left = 0;
	if (rect.top < 0) rect.top = 0;
	if (rect.left + rect.width > world.width) rect.left = world.width - w;
	if (rect.top + rect.height > world.height) rect.top = world.height - h;

	game_view->reset(rect);
}

void ScenePlay::sGameState() {
	if (!player->alive && player->get<CAnimation>()->active_anim->hasEnded() ||
		!base->alive && base->get<CAnimation>()->active_anim->hasEnded())
	{
		game_stats->state = GameStats::State::LOSE;
		game_stats->addKills(kills_per_enemy);
		game_stats->next_stage = 0;
		setFade(FADE::OUT, GAME_SCENE::OVER);
	}

	if (ent_mgr.getEntities(TAG::ENEMY).empty()) {
		game_stats->state = GameStats::State::WIN;
		game_stats->addKills(kills_per_enemy);
		if (game_stats->stageNext()) {
			setFade(FADE::OUT, GAME_SCENE::SCORE);
		}
		else {
			game_stats->next_stage = 0;
			setFade(FADE::OUT, GAME_SCENE::OVER);
		}
	}
#ifdef DEBUG_ENEMIES_LEFT
	else {
		std::cout << "enemies left: " << ent_mgr.getEntities(TAG::ENEMY).size() << std::endl;
	}
#endif
}

float ScenePlay::squareDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}
