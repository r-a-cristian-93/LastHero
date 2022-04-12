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
	}

	file.close();

	//load texture after creating the window causes sementation fault;
	//assets = Assets();

	screen_tex.create(app_conf.window_w, app_conf.window_h);
	screen_tex.setView(view);
	screen_sprite = sf::Sprite(screen_tex.getTexture());

	window.create(sf::VideoMode(app_conf.window_w, app_conf.window_h), app_conf.window_name, app_conf.window_style);
	window.setFramerateLimit(app_conf.max_fps);
	window.setKeyRepeatEnabled(false);

	act_mgr = ActionManager();

	setScene(GAME_SCENE_MENU);

	running = true;
}

void Game::run() {
	sf::Event event;
	sf::FloatRect lim(0,0,app_conf.window_w,app_conf.window_h);

	while(running) {
		PROFILE_SCOPE("MAIN_GAME_LOOP");

		if (window.isOpen()) {
			screen_tex.clear();
			window.clear();

			sUserInput();

			current_scene->update();

			screen_tex.display();

			//window.draw(screen_sprite, &assets->getShader("crt-geom"));
			{
				PROFILE_SCOPE("window.draw()");
				window.draw(screen_sprite);
			}
			{
				PROFILE_SCOPE("window.display()");
				window.display();
			}
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
	act_mgr = ActionManager();

	Scene* old = current_scene;

	switch (id) {
		case GAME_SCENE_MENU:
			current_scene = new SceneMainMenu(this);
		break;
		case GAME_SCENE_PLAY:
			current_scene = new ScenePlay(this, stages[next_stage]);
			if (next_stage == 0) {
				kills_per_enemy.clear();
				new_kills_per_enemy.clear();
			}
		break;
		case GAME_SCENE_OVER:
			current_scene = new SceneGameOver(this);
		break;
		case GAME_SCENE_SCORE:
			current_scene = new SceneScore(this);
		break;
	}

	if (old) delete old;
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

}
