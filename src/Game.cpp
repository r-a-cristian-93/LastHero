#include <fstream>
#include <iostream>
#include <cmath>

#include "Game.h"
#include "SDraw.h"
#include "SUpdate.h"
#include "Assets.h"
#include "ScenePlay.h"
#include "SceneMainMenu.h"
#include "SceneGameOver.h"
#include "SceneScore.h"
#include "SceneSettings.h"

Game::Game(std::string file_name)
	:running(false)
	,current_scene(nullptr)
	,next_stage(0)
	,prev_stage(0)
	,next_scene(0)
{
	init(file_name);
}

void Game::init(std::string file_name) {
	PROFILE_FUNCTION();

	std::ifstream file(file_name);
	std::string word;

	while(file >> word) {
		if (word == "WINDOW_NAME") {
			file >> app_conf.window_name;
		}
		if (word == "WINDOW_STYLE") {
			file >> app_conf.window_style;
		}
		if (word == "MAX_FPS") {
			file >> app_conf.max_fps;
		}
		if (word == "WINDOW_RES") {
			size_t mode_id = 0;
			file >> mode_id;
			if (mode_id < app_conf.modes.size()) {
				app_conf.current_mode_id = mode_id;
			}
			else {
				std::cout << "ERROR: Resolution not supported\n";
			}
		}
		if (word == "GAME_RES") {
			file >> app_conf.game_w;
			file >> app_conf.game_h;
		}
		if (word == "CAM_SPEED") {
			file >> app_conf.cam_speed;
		}
		if (word == "CAM_TRESHOLD") {
			file >> app_conf.cam_treshold;
		}
		if (word == "STAGE") {
			file >> word;
			stages.push_back(word);
		}
		if (word == "COLMAP_RES") {
			file >> app_conf.colmap_res;
		}
		if (word == "COLMAP_UPDATE") {
			file >> app_conf.colmap_update;
		}
		if (word == "FADE_SCENE") {
			int frames = 1;

			for (size_t f_type=1; f_type<FADE::COUNT; f_type++) {
				file >> frames;
				if (frames <= 0) {
					frames = 1;
					std::cout << "FADE_SCENE frames must be greater than 0. Set to default value 1.\n";
				}
				if (frames > 255) {
					frames = 255;
					std::cout << "FADE_SCENE frames must be lower than 256. Set to default value 1.\n";
				}

				app_conf.scene_fade_frames[f_type] = frames;
			}
		}
		if (word == "FRAMES_SCORE") {
			int frames = 1;

			for (size_t f_type=1; f_type<FRAMES_SCORE::COUNT; f_type++) {
				file >> frames;
				if (frames <= 0) {
					frames = 1;
					std::cout << "FRAMES_SCORE frames must be greater than 0. Set to default value 1.\n";
				}

				app_conf.score_key_frames[f_type] = frames;
			}
		}
		if (word == "FADE_MULTIPLYER") {
			float m = 1;
			file >> m;

			if (m <= 0) {
				m = 1;
				std::cout << "Fade multipilier must be greater than 0. Set to default value 1.\n";
			}
			app_conf.fade_multiplier = m;

			// apply multiplier
			for (size_t f=0; f<FADE::COUNT; f++) {
				app_conf.scene_fade_frames[f] *= m;
			}
			for (size_t f=0; f<FRAMES_SCORE::COUNT; f++) {
				app_conf.score_key_frames[f] *= m;
			}
		}
	}

	file.close();

	//load texture after creating the window causes sementation fault;
	screen_tex.create(app_conf.game_w, app_conf.game_h);
	screen_sprite = sf::Sprite(screen_tex.getTexture(), {0, 0, app_conf.game_w, app_conf.game_h});

	applySettings(app_conf);

	act_mgr = ActionManager();
	snd_mgr = SoundManager(&assets);

	setScene(GAME_SCENE::MENU);

	running = true;
}

void Game::reset(sf::Sprite& sprite) {
	sprite.setScale(1,1);
	sprite.setPosition(0,0);
}

void Game::fit(sf::Sprite& sprite) {
	float scale = static_cast<float>(app_conf.modes[app_conf.current_mode_id].height) / app_conf.game_h;
	scale *= 0.95;
	sprite.setScale(scale,scale);

	float offset_x = (app_conf.modes[app_conf.current_mode_id].width - screen_sprite.getGlobalBounds().width)/2;
	float offset_y = (app_conf.modes[app_conf.current_mode_id].height - screen_sprite.getGlobalBounds().height)/2;

	sprite.setPosition(offset_x, offset_y);
}

void Game::run() {
	sf::Event event;
	sf::FloatRect lim(0,0,app_conf.modes[app_conf.current_mode_id].width, app_conf.modes[app_conf.current_mode_id].height);

	while(running) {
		PROFILE_SCOPE("MAIN_GAME_LOOP");

		if (window.isOpen()) {
			screen_tex.clear();

			if (current_scene) current_scene->update();

			screen_tex.display();

			window.clear();
			window.draw(screen_sprite, &assets.getShader("crt-mattias"));
			//window.draw(screen_sprite);
			window.display();

			sUserInput();
			sChangeScene();
		}
	}
}

void Game::sUserInput() {
	PROFILE_FUNCTION();

	sf::Event event;
	Action* action = nullptr;
	int action_code = Action::NONE;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			running = false;
		}
		else if (event.type == sf::Event::KeyPressed) {
			action_code = act_mgr.getCode(ActionManager::DEV_KEYBOARD, event.key.code);

			if (action_code) {
				action = new Action(action_code, Action::TYPE_START);
				current_scene->doAction(action);
				delete action;
			}
		}
		else if (event.type == sf::Event::KeyReleased) {
			action_code = act_mgr.getCode(ActionManager::DEV_KEYBOARD, event.key.code);

			if (action_code) {
				action = new Action(action_code, Action::TYPE_END);
				current_scene->doAction(action);
				delete action;
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed){
			action_code = act_mgr.getCode(ActionManager::DEV_MOUSE, event.mouseButton.button);

			if (action_code != 0) {
				action = new Action(action_code, Action::TYPE_START, sf::Mouse::getPosition());
				current_scene->doAction(action);
				delete action;
			}
		}
	}
}

void Game::sChangeScene() {
	if (next_scene) {
		setScene(next_scene);
		next_scene = 0;
	}
}

void Game::setScene(size_t id) {
	delete current_scene;
	act_mgr = ActionManager();

	switch (id) {
		case GAME_SCENE::MENU:
			current_scene = new SceneMainMenu(this);
		break;
		case GAME_SCENE::SETTINGS:
			current_scene = new SceneSettings(this);
		break;
		case GAME_SCENE::PLAY:
			current_scene = new ScenePlay(this, stages[next_stage]);
			if (next_stage == 0) {
				kills_per_enemy.clear();
				new_kills_per_enemy.clear();
			}
		break;
		case GAME_SCENE::OVER:
			current_scene = new SceneGameOver(this);
		break;
		case GAME_SCENE::SCORE:
			current_scene = new SceneScore(this);
		break;
		case GAME_SCENE::EXIT:
			current_scene = nullptr;
			running = false;
		break;
		default:
			std::cout << __FILE__ << ":" << __LINE__ << " ERROR : Scene id " << id << " not handled\n";
		break;
	}
}

void Game::setNextScene(size_t id) {
	next_scene = id;
}

void Game::addKills(std::map<size_t, size_t> kills) {
	new_kills_per_enemy = kills;

	KillsMap::const_iterator it;
	for (it = kills.cbegin(); it!=kills.cend(); it++) {
		kills_per_enemy[it->first] += it->second;
	}
}

bool Game::stageNext() {
	prev_stage = next_stage;
	if (next_stage < stages.size()-1) {
		next_stage++;

		return true;
	}
	return false;
}

void Game::stageReset() {
	next_stage = 0;
}

size_t Game::stageCurrent() {
	return next_stage;
}

size_t Game::stagePrev() {
	return prev_stage;
}

size_t Game::stagesCount() {
	return stages.size();
}

void Game::applySettings(AppConfig& conf) {
	app_conf = conf;

	fit(screen_sprite);
	window.create(app_conf.modes[app_conf.current_mode_id], app_conf.window_name, app_conf.window_style);
	window.setFramerateLimit(app_conf.max_fps);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursorVisible(false);
}

Game::~Game() {
	delete current_scene;
}
