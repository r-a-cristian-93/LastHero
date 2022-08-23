#include <fstream>
#include <iostream>
#include <cmath>

#include "Game.h"
#include "SDraw.h"
#include "SUpdate.h"
#include "ScenePlay.h"
#include "SceneMainMenu.h"
#include "SceneGameOver.h"
#include "SceneScore.h"
#include "SceneSettings.h"
#include "SceneEditor.h"

Game::Game()
	:running(false)
	,current_scene(nullptr)
	,next_stage(0)
	,prev_stage(0)
	,next_scene(0)
{
	init();
}

void Game::init() {
	PROFILE_FUNCTION();

	// load default settings
	app_conf.read("game.cfg");

	// load user settings
	app_conf.read("user.cfg");

	//load texture after creating the window causes sementation fault;
	screen_tex.create(app_conf.game_w, app_conf.game_h);
	screen_sprite = sf::Sprite(screen_tex.getTexture(), {0, 0, app_conf.game_w, app_conf.game_h});

	applySettings(app_conf);

	act_mgr = ActionManager();
	snd_mgr = SoundManager(&assets, &app_conf);

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
	int action_code = Action::NONE;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			running = false;
		}
		else if (event.type == sf::Event::KeyPressed) {
			action_code = act_mgr.getCode(ActionManager::DEV_KEYBOARD, event.key.code);

			if (action_code) {
				Action action(action_code, Action::TYPE_START);
				current_scene->doAction(action);
			}
		}
		else if (event.type == sf::Event::KeyReleased) {
			action_code = act_mgr.getCode(ActionManager::DEV_KEYBOARD, event.key.code);

			if (action_code) {
				Action action(action_code, Action::TYPE_END);
				current_scene->doAction(action);
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed){
			action_code = act_mgr.getCode(ActionManager::DEV_MOUSE, event.mouseButton.button);

			if (action_code != 0) {
				Action action(action_code, Action::TYPE_START, sf::Mouse::getPosition());
				current_scene->doAction(action);
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
			current_scene = new ScenePlay(this, assets.getStages()[next_stage]);
			if (next_stage == 0) {
				kills_per_enemy.clear();
				new_kills_per_enemy.clear();
			}
		break;
		case GAME_SCENE::EDITOR:
			current_scene = new SceneEditor(this);
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
	if (next_stage < assets.getStages().size()-1) {
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
	return assets.getStages().size();
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
