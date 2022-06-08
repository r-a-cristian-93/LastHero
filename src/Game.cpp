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
		if (word == "Window") {
			file >> app_conf.window_name;
			file >> app_conf.window_w;
			file >> app_conf.window_h;
			file >> app_conf.max_fps;
			int style_bits;
			file >> style_bits;
			app_conf.window_style = 1 << style_bits;
		}
		if (word == "Game") {
			file >> app_conf.game_w;
			file >> app_conf.game_h;
		}
		if (word == "Camera") {
			file >> app_conf.cam_speed >> app_conf.cam_treshold;
		}
		if (word == "stage") {
			file >> word;
			stages.push_back(word);
		}
		if (word == "Colmap") {
			file >> app_conf.colmap_res;
			file >> app_conf.colmap_update;
		}
		if (word == "Scene") {
			size_t scene_type = 0, in = 1, out = 1;
			file >> word >> in >> out;

			if (in <= 0) {
				in = 1;
				std::cout << "Fade in frames of scene " << word << " must be greater than 0. Set to default value 1.\n";
			}
			if (out <= 0) {
				out = 1;
				std::cout << "Fade out frames of scene " << word << " must be greater than 0. Set to default value 1.\n";
			}
			if (in > 255) {
				in = 255;
				std::cout << "Fade in frames of scene " << word << " must be lower than 256. Set to default value 255.\n";
			}
			if (out > 255) {
				out = 255;
				std::cout << "Fade out frames of scene " << word << " must be lower than 255. Set to default value 255.\n";
			}

			if (word == "MENU") scene_type = GAME_SCENE::MENU;
			else if (word == "PLAY") scene_type = GAME_SCENE::PLAY;
			else if (word == "OVER") scene_type = GAME_SCENE::OVER;
			else if (word == "SCORE") scene_type = GAME_SCENE::SCORE;
			else scene_type = GAME_SCENE::NONE;

			app_conf.scene_fade_frames[scene_type][FADE::IN] = in;
			app_conf.scene_fade_frames[scene_type][FADE::OUT] = out;
		}
		if (word == "FADE_MULTIPLYER") {
			float m = 1;
			file >> m;

			if (m <= 0) {
				m = 1;
				std::cout << "Fade multipilier must be greater than 0. Set to default value 1.\n";
			}

			app_conf.fade_multiplier = m;
		}
	}

	file.close();

	//load texture after creating the window causes sementation fault;
	screen_tex.create(app_conf.game_w, app_conf.game_h);
	screen_sprite = sf::Sprite(screen_tex.getTexture(), {0, 0, app_conf.game_w, app_conf.game_h});
	fit(screen_sprite);
	window.create(sf::VideoMode(app_conf.window_w, app_conf.window_h), app_conf.window_name, app_conf.window_style);
	window.setFramerateLimit(app_conf.max_fps);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursorVisible(false);

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
	float scale = static_cast<float>(app_conf.window_h) / app_conf.game_h;
	scale *= 0.95;
	sprite.setScale(scale,scale);

	float offset_x = (app_conf.window_w - screen_sprite.getGlobalBounds().width)/2;
	float offset_y = (app_conf.window_h - screen_sprite.getGlobalBounds().height)/2;

	sprite.setPosition(offset_x, offset_y);
}

void Game::run() {
	sf::Event event;
	sf::FloatRect lim(0,0,app_conf.window_w,app_conf.window_h);

	while(running) {
		PROFILE_SCOPE("MAIN_GAME_LOOP");

		if (window.isOpen()) {
			screen_tex.clear();

			if (current_scene) current_scene->update();

			screen_tex.display();

			window.clear();
			window.draw(screen_sprite, &assets.getShader("crt-mattias"));
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
	act_mgr = ActionManager();

	delete current_scene;

	switch (id) {
		case GAME_SCENE::MENU:
			current_scene = new SceneMainMenu(this);
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
			running = false;
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

Game::~Game() {
	delete current_scene;
}
