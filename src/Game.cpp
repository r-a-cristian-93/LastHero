#include <fstream>
#include <iostream>
#include <cmath>

#include "Game.h"
#include "SDraw.h"
#include "SUpdate.h"
#include "Assets.h"
#include "ScenePlay.h"

Game::Game(std::string file_name)
	:running(false)
{
	init(file_name);
}

void Game::init(std::string file_name) {
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
	}

	file.close();

	//load texture after creating the window causes sementation fault;
	assets = new Assets();

	screen_tex.create(app_conf.window_w, app_conf.window_h);
	screen_tex.setView(view);
	screen_sprite = sf::Sprite(screen_tex.getTexture());

	window.create(sf::VideoMode(app_conf.window_w, app_conf.window_h), app_conf.window_name, app_conf.window_style);
	window.setFramerateLimit(app_conf.max_fps);
	window.setKeyRepeatEnabled(false);

	act_mgr = ActionManager();
	scenes[2] = new ScenePlay(this, "res/level_001.cfg");

	setScene(2);

	running = true;
}

void Game::run() {
	sf::Event event;

	sf::FloatRect lim(0,0,app_conf.window_w,app_conf.window_h);

	while(running) {
		if (window.isOpen()) {
			screen_tex.clear();
			window.clear();

			current_scene->update();
			sUserInput();

			screen_tex.display();
			//window.draw(screen_sprite, &assets->getShader("pixelizer"));
			window.draw(screen_sprite);
			window.display();
		}
	}
}

void Game::sUserInput() {
	sf::Event event;
	Action* action = nullptr;
	int action_code = Action::NONE;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			running = false;
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			action_code = act_mgr.getCode(ActionManager::DEV_KEYBOARD, event.key.code);

			if (action_code) {
				action = new Action(action_code, Action::TYPE_START);
				current_scene->doAction(action);
			}
		}
		else if (event.type == sf::Event::KeyReleased) {
			action_code = act_mgr.getCode(ActionManager::DEV_KEYBOARD, event.key.code);

			if (action_code) {
				action = new Action(action_code, Action::TYPE_END);
				current_scene->doAction(action);
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed){
			action_code = act_mgr.getCode(ActionManager::DEV_MOUSE, event.mouseButton.button);

			if (action_code != 0) {
				action = new Action(action_code, Action::TYPE_START, sf::Mouse::getPosition());
				current_scene->doAction(action);
			}
		}
	}
}

void Game::setScene(size_t id) {
	current_scene = scenes[id];
}

Game::~Game() {
	for (size_t i=0; i<scenes.size(); i++) {
		if (scenes[i]) {
			delete scenes[i];
		}
	}

	scenes.clear();
}
