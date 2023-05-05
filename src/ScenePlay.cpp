#include "ScenePlay.h"
#include "SharedResources.h"
#include "Profiler.h"

ScenePlay::ScenePlay(std::string level_path)
	:ScenePlay(GAME_SCENE::PLAY, level_path)
{
	init();
}

ScenePlay::ScenePlay(size_t t, std::string level_path)
	:Scene(t)
	,play_data(level_path)
	,sView(game_view, play_data)
	,sStateFacing(play_data)
	,sEntityPosition(play_data)
	,sLifespan(play_data)
	,sAnimation(play_data)
	,sDrawEntities(play_data)
	,sPowerup(play_data)
	,sCollision(play_data)
	,sSpawnFx(play_data)
	,sWidgetFx(interface, play_data)
	,sFireWeapon(play_data)
	,sAI(play_data)
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
			sSpawnFx();

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

	//change view in order to keep the interface relative to window
	{
		PROFILE_SCOPE("sDrawInterface");
		screen_tex->setView(gui_view);
		sDrawInterface();
		screen_tex->setView(game_view);
	}

	frame_current += frame_mgr->getIncrement();
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