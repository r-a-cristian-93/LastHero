#include <fstream>
#include <iostream>
#include <cmath>

#include "Game.h"
#include "SDraw.h"
#include "SUpdate.h"
#include "Assets.h"
#include "ScenePlay.h"

float squareDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}

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
/*		if (word == "Font") {
			std::string font_file_path;
			file >> font_file_path;
			file >> app_conf.font_size;
			file >> app_conf.font_r;
			file >> app_conf.font_g;
			file >> app_conf.font_b;
			if (!font.loadFromFile(font_file_path)) {
				std::cerr << "Could not load font!\n";
				exit(-1);
			}
		}*/
	}

	file.close();

	window.create(sf::VideoMode(app_conf.window_w, app_conf.window_h), app_conf.window_name, app_conf.window_style);
	window.setFramerateLimit(app_conf.max_fps);
	window.setKeyRepeatEnabled(false);
/*
	score_text = sf::Text("0", font, app_conf.font_size);
	score_text.setFillColor(sf::Color(app_conf.font_r, app_conf.font_g, app_conf.font_b));
	score_text.setPosition(20.0f, 20.0f);
*/

	act_mgr = ActionManager();
	assets = new Assets();
	scenes[2] = new ScenePlay(this, "res/level_001.cfg");

	setScene(2);

	running = true;
}

void Game::run() {
	sf::Event event;

	sf::FloatRect lim(0,0,app_conf.window_w,app_conf.window_h);

	while(running) {
		if (window.isOpen()) {

			window.clear();

			current_scene->update();
			sUserInput();

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
	std::cout << "scene set:" << id << std::endl;
}
