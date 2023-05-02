#include "ScenePlay.h"
#include <cmath>
#include "SharedResources.h"
#include "Profiler.h"
#include "CheckCollision.h"

ScenePlay::ScenePlay(std::string level_path)
	:ScenePlay(GAME_SCENE::PLAY, level_path)
{
	init();
}

ScenePlay::ScenePlay(size_t t, std::string level_path)
	:Scene(t)
	,play_data(level_path)
	,sView(play_data)
	,sStateFacing(play_data)
	,sEntityPosition(play_data)
	,sLifespan(play_data)
	,sAnimation(play_data)
	,sDrawEntities(play_data)
	,sPowerup(play_data)
	,sCollision(play_data)
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

	load_level(play_data.level_path);
	play_data.collision_map.setMap(play_data.level.map_size, play_data.level.tile_size, app_conf->colmap_res, app_conf->colmap_update);

	// setup interface
	interface.add(assets->getWidget("play_ui"));
	interface.add(assets->getWidget("blood_overlay"));

	std::variant<std::monostate, int*, std::string*> links[static_cast<int>(Link::Target::COUNT)];
	links[static_cast<int>(Link::Target::PLAYER_HP)] = &play_data.player->get<CStats>()->effective[CStats::HEALTH];
	links[static_cast<int>(Link::Target::BASE_HP)] = &play_data.base->get<CStats>()->effective[CStats::HEALTH];
	links[static_cast<int>(Link::Target::TOTAL_KILLS)] = &play_data.total_kills;
	links[static_cast<int>(Link::Target::SECONDARY_ROUNDS)] = &play_data.player->get<CWeapon>()->s_rounds;
	links[static_cast<int>(Link::Target::SECONDARY_ROUNDS_CURRENT)] = &play_data.player->get<CWeapon>()->s_rounds_current;
	interface.setLinks(links);

	play_data.paused_widget = &assets->getWidget("menu_paused");
	sf::Vector2i pos;
	pos.x = static_cast<int>(app_conf->game_w*0.5);
	pos.y = static_cast<int>(app_conf->game_h*0.5);
	play_data.paused_widget->setPosAbs(pos);

	// run this block to display level;
	{
		play_data.ent_mgr.update();
		sEntityPosition();
		sCollision();
		sStateFacing();
		sFireWeapon();
		sAnimation();

		// focus camera on player
		play_data.cam.pos = play_data.player->get<CTransform>()->pos;

		interface.update();
	}
}

void ScenePlay::load_level(std::string path) {
	play_data.level = Level(path);
	snd_mgr->playBgMusic(play_data.level.bg_music);

	for (Action& a: play_data.level.actions) {
		doAction(a);
	}

	play_data.ent_mgr.update();
}

void ScenePlay::drawCollisionLayer() {
	sf::CircleShape circle(play_data.level.tile_size.x/(app_conf->colmap_res*2));

	//DRAW BLOCKING
	if (!play_data.collision_map.colmap.empty()) {
		circle.setFillColor(sf::Color(255, 0, 0, 50));

		for (int y = 0; y < play_data.collision_map.map_size.x; y++) {
			for (int x = 0; x < play_data.collision_map.map_size.x; x++) {
				circle.setPosition(sf::Vector2f(x*play_data.level.tile_size.x/app_conf->colmap_res,y*play_data.level.tile_size.y/app_conf->colmap_res));

				if (play_data.collision_map.colmap[x][y]) {
					screen_tex->draw(circle);
				}
			}
		}
	}

	//DRAW PATHS
	circle.setFillColor(sf::Color(0, 255, 255, 80));

	for (std::shared_ptr<Entity>& e: play_data.ent_mgr.getEntities()) {
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
	for (std::shared_ptr<Entity>& e: play_data.ent_mgr.getEntities()) {
		if (e->get<CTransform>()) {
			if (e->get<CTransform>()->max_velocity) {
				sf::Vector2f dir = dirOf(e->facing);
				dir.x *= play_data.level.tile_size.x/2;
				dir.y *= play_data.level.tile_size.x/2;

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
	rect.setSize(sf::Vector2f(play_data.level.tile_size));
	rect.setFillColor(sf::Color(0,0,0,0));
	rect.setOutlineColor(sf::Color(0,0,0,50));
	rect.setOutlineThickness(-1);

	for (size_t x = 0; x < play_data.level.map_size.x; x++) {
		for (size_t y = 0; y < play_data.level.map_size.y; y++) {
			rect.setPosition(sf::Vector2f(x*play_data.level.tile_size.x, y*play_data.level.tile_size.y));
			screen_tex->draw(rect);
		}
	}
}

void ScenePlay::drawEntityPosition() {
	sf::Color col(255,0,0);
	for (std::shared_ptr<Entity>& e : play_data.ent_mgr.getEntities()) {
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
			// commands from player: App::sUserInput(), calls doAction()

			// commands from AI
			sAI();

			// remove dead entities, add new spawn entities
			play_data.ent_mgr.update();

			// update collision map
			play_data.collision_map.update();

			// move entities
			sEntityPosition();

			// create inter-entities collisions
			sCollision();

			// applies powerups
			sPowerup();

			// set widget fx based on game state
			sWidgetFx();

			// set entity state and facing
			sStateFacing();

			// handle weapon firing and spawns projectiles
			sFireWeapon();

			// updated the interface (text, fx)
			interface.update();

			// check if entities ran out of time and kill them
			sLifespan();

			// handle entity animation (set, advance)
			sAnimation();

			// spawn fx
			sPlayFx();

			// check level end conditions and set next scene
			sGameState();
		}
		sView();
	}

	{
		PROFILE_SCOPE("sDrawBg");
		screen_tex->draw(play_data.level.map_ground);
	}

	{
		PROFILE_SCOPE("sDrawEntities");
		sDrawEntities();
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

	//change view in order to keep the interface relative to window
	{
		PROFILE_SCOPE("sDrawInterface");
		screen_tex->setView(gui_view);
		sDrawInterface();
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

	std::shared_ptr<Entity> e = play_data.ent_mgr.add(TAG::ENEMY);
	// deprecated
	int radius = 50;

	while (!position_is_valid) {
		pos.x = rand() % static_cast<int>(app_conf->modes[app_conf->current_mode_id].width - radius*2) + radius;
		pos.y = rand() % static_cast<int>(app_conf->modes[app_conf->current_mode_id].height - radius*2) + radius;

		float square_min_dist = (player_radius*10 + radius) * (player_radius*10 + radius);
		float square_current_dist = squareDistance(pos, play_data.player->get<CTransform>()->pos);

		if (square_current_dist > square_min_dist) {
			position_is_valid = true;
		}
	}

	e->get<CTransform>()->pos = pos;
	e->get<CTransform>()->dir = dir;
	e->get<CStats>()->level = rand() % 10;
}

void ScenePlay::sEnemySpawner() {
	if (frame_current % 100 == 0) {
		spawnEnemy();
	}
}

void ScenePlay::sPlayFx() {
	for (std::shared_ptr<Entity>& e : play_data.ent_mgr.getEntities(TAG::PROJECTILE)) {
		 if (e->get<CAnimation>() && e->get<CFx>()) {
			 for (Fx fx : e->get<CFx>()->fx) {
				 switch (fx.trigger) {
					case TR::DIE:
						if (!e->alive && e->get<CAnimation>()->active_anim->hasEnded()) {
							play_data.ent_mgr.spawnEntity(fx.tag, fx.id, e->get<CTransform>()->pos, Entity::STATE_DIE, Entity::FACING_E);
						}
					break;
				 }
			 }
		}
	}
}

void ScenePlay::sAI() {
	for (std::shared_ptr<Entity>& e : play_data.ent_mgr.getEntities()) {
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
				bool has_path = play_data.collision_map.computePath(start, end, path, MapCollision::MOVE_NORMAL, 0);

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

				if (CheckCollision::of(e, e->get<CBChase>()->target, range*0.2)) {
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

void ScenePlay::handleChase(std::shared_ptr<Entity>& e, const BCondition& bc) {
	switch (bc.trigger) {
		case TR::PLAYER_NEARBY:
		{
			if (CheckCollision::of(e, play_data.player, bc.data_start)) {
				e->get<CBChase>()->target = play_data.player;
			}
			// lose agro if target got too far
			else if (!CheckCollision::of(e, play_data.player, bc.data_stop)) {
				if (e->get<CBChase>()->target = play_data.player) {
					e->get<CBChase>()->target = nullptr;
					e->get<CBChase>()->path.clear();
				}
			}
		}
		break;
		case TR::BASE_NEARBY:
			if (CheckCollision::of(e, play_data.base, bc.data_start)) {
				e->get<CBChase>()->target = play_data.base;
			}
			// lose agro if target got too far
			else if (!CheckCollision::of(e, play_data.base, bc.data_stop)) {
				if (e->get<CBChase>()->target == play_data.base) {
					e->get<CBChase>()->target = nullptr;
					e->get<CBChase>()->path.clear();
				}
			}
		break;
		case TR::BASE_NOT_PROTECTED:
			if (!CheckCollision::of(play_data.player, play_data.base, bc.data_start)) {
				e->get<CBChase>()->target = play_data.base;
			}
			// lose agro if target got too far
			else if (CheckCollision::of(play_data.player, play_data.base, bc.data_stop)) {
				if (e->get<CBChase>()->target == play_data.base) {
					e->get<CBChase>()->target = nullptr;
					e->get<CBChase>()->path.clear();
				}
			}
		break;
		case TR::BASE_LOW_HP:
			if (play_data.base->get<CStats>()->effective[CStats::HEALTH] < play_data.base->get<CStats>()->initial[CStats::HEALTH] * bc.data_start / 100) {
				e->get<CBChase>()->target = play_data.base;
			}
		break;
	}
}

void ScenePlay::handleFire(std::shared_ptr<Entity>& e, const BCondition& bc, bool& fire_weapon) {
	// MUST CHECK COLLISION BETWEEN PROJECTILE SPAWN POSTION AND TARGET
	// ADD NEW CheckCollision::of() FUNCTION

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
			if (CheckCollision::of(e, play_data.player, bc.data_start)) {
				fire_weapon = true;
				e->get<CBFire>()->target = play_data.player;
			}
		break;
		case TR::BASE_NEARBY:
			if (CheckCollision::of(e, play_data.base, bc.data_start)) {
				fire_weapon = true;
				e->get<CBFire>()->target = play_data.base;
			}
		break;
	}

	if (e->get<CBChase>()) {
		if (e->get<CBChase>()->target) {
			e->get<CBFire>()->target = e->get<CBChase>()->target;
		}
	}

	if (e->get<CBFire>()->target) {
		e->facing = facingOf(e->get<CBFire>()->target->get<CTransform>()->pos - e->get<CTransform>()->pos);
	}
}

void ScenePlay::sFireWeapon() {
	for (std::shared_ptr<Entity>& e : play_data.ent_mgr.getEntities()) {
		if (e->get<CWeapon>() && e->alive) {
			CWeapon& comp_w = *e->get<CWeapon>();
			size_t facing(e->facing);

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
						play_data.ent_mgr.spawnEntity(comp_w.p_tag, comp_w.primary, e, pos, Entity::STATE_RUN, facing);
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
						play_data.ent_mgr.spawnEntity(comp_w.s_tag, comp_w.secondary, e, pos, Entity::STATE_RUN, facing);
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

void ScenePlay::spawnMissle() {
	sf::Vector2f mouse_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	const sf::Vector2f pos(play_data.player->get<CTransform>()->pos);
	const sf::Vector2f dir = mouse_pos - pos;

	std::shared_ptr<Entity> missle = play_data.ent_mgr.add(TAG::MISSLE);

	missle->get<CTransform>()->pos = pos;
	missle->get<CTransform>()->dir = dir;
	missle->get<CShape>()->shape.setPosition(pos);
}

std::shared_ptr<Entity> ScenePlay::findTarget(const std::shared_ptr<Entity>& missle) {
	EntityVec reachable;
	sf::Vector2f dir_missle(missle->get<CTransform>()->dir);
	sf::Vector2f dir_enemy;

	for (std::shared_ptr<Entity>& enemy : play_data.ent_mgr.getEntities(TAG::ENEMY)) {
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
	for (std::shared_ptr<Entity>& missle : play_data.ent_mgr.getEntities(TAG::MISSLE)) {

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
				interface.add(*play_data.paused_widget);
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
					play_data.player->get<CInput>()->up = true;
				break;
				case Action::MOVE_LEFT:
					play_data.player->get<CInput>()->left = true;
				break;
				case Action::MOVE_DOWN:
					play_data.player->get<CInput>()->down = true;
				break;
				case Action::MOVE_RIGHT:
					play_data.player->get<CInput>()->right = true;
				break;
				case Action::FIRE_PRIMARY:
					play_data.player->get<CInput>()->fire_primary = true;
				break;
				case Action::FIRE_SECONDARY:
					play_data.player->get<CInput>()->fire_secondary = true;
				break;
				case Action::CHANGE_SCENE_MENU:
					setFade(FADE::OUT, GAME_SCENE::MENU);
				break;
				case Action::SPAWN_ENTITY:
					play_data.ent_mgr.spawnEntity(*a.ent_tag, *a.ent_name, *a.pos, *a.state, *a.facing);
				break;
				case Action::SPAWN_PLAYER:
					play_data.ent_mgr.spawnEntity(*a.ent_tag, *a.ent_name, *a.pos, *a.state, *a.facing);
					play_data.ent_mgr.update();
					play_data.player = play_data.ent_mgr.getEntities(TAG::PLAYER)[0];
				break;
				case Action::SPAWN_BASE:
					play_data.ent_mgr.spawnEntity(*a.ent_tag, *a.ent_name, *a.pos, *a.state, *a.facing);
					play_data.ent_mgr.update();
					play_data.base = play_data.ent_mgr.getEntities(TAG::BASE)[0];
				default:
				break;
			}
		}
	}
	if (*a.type == Action::TYPE_END) {
		switch (*a.code) {
			case Action::MOVE_UP:
				play_data.player->get<CInput>()->up = false;
			break;
			case Action::MOVE_LEFT:
				play_data.player->get<CInput>()->left = false;
			break;
			case Action::MOVE_DOWN:
				play_data.player->get<CInput>()->down = false;
			break;
			case Action::MOVE_RIGHT:
				play_data.player->get<CInput>()->right = false;
			break;
			default:
			break;
		}
	}
}

void ScenePlay::sWidgetFx() {
	WidgetVec& widgets = interface.getWidgets();

	for (Widget& w: widgets) {
		handleWidgetFx(w);
	}
}

void ScenePlay::handleWidgetFx(Widget& w) {
	for (WidgetFx& wfx: w.fx) {
		if (wfx.cond.trigger == TR::PLAYER_HURT && play_data.player->hit) {
			w.current_fx = &wfx;
		}
	}

	if (!w.childs.empty()) {
		for (Widget& child: w.getChilds()) {
			handleWidgetFx(child);
		}
	}
}

void ScenePlay::sGameState() {
	if (!play_data.player->alive && play_data.player->get<CAnimation>()->active_anim->hasEnded() ||
		!play_data.base->alive && play_data.base->get<CAnimation>()->active_anim->hasEnded())
	{
		game_stats->state = GameStats::State::LOSE;
		game_stats->addKills(play_data.kills_per_enemy);
		game_stats->next_stage = 0;
		setFade(FADE::OUT, GAME_SCENE::OVER);
	}

	if (play_data.ent_mgr.getEntities(TAG::ENEMY).empty()) {
		game_stats->state = GameStats::State::WIN;
		game_stats->addKills(play_data.kills_per_enemy);
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
		std::cout << "enemies left: " << play_data.ent_mgr.getEntities(TAG::ENEMY).size() << std::endl;
	}
#endif
}

float ScenePlay::squareDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}
