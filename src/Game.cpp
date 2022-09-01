#include <iostream>

#include "Game.h"
#include "ScenePlay.h"
#include "SceneMainMenu.h"
#include "SceneGameOver.h"
#include "SceneScore.h"
#include "SceneSettings.h"

Game::Game()
{
	init();
}

void Game::init() {
	//load texture after creating the window causes sementation fault;
	screen_tex->create(app_conf->game_w, app_conf->game_h);
	screen_sprite->setTextureRect({0, 0, app_conf->game_w, app_conf->game_h});

	{
		float scale = static_cast<float>(app_conf->modes[app_conf->current_mode_id].height) / app_conf->game_h;
		scale *= app_conf->game_scale;
		screen_sprite->setScale(scale,scale);

		float offset_x = (app_conf->modes[app_conf->current_mode_id].width - screen_sprite->getGlobalBounds().width)/2;
		float offset_y = (app_conf->modes[app_conf->current_mode_id].height - screen_sprite->getGlobalBounds().height)/2;

		screen_sprite->setPosition(offset_x, offset_y);
	}

	window->create(app_conf->modes[app_conf->current_mode_id], app_conf->window_name, app_conf->window_style);
	window->setFramerateLimit(app_conf->max_fps);
	window->setKeyRepeatEnabled(false);
	window->setMouseCursorVisible(false);

	setScene(GAME_SCENE::MENU);
}

void Game::setScene(size_t id) {
	delete current_scene;
	act_mgr->reset();

	switch (id) {
		case GAME_SCENE::MENU:
			current_scene = new SceneMainMenu();
		break;
		case GAME_SCENE::SETTINGS:
			current_scene = new SceneSettings();
		break;
		case GAME_SCENE::PLAY:
			current_scene = new ScenePlay(assets->getStages()[game_stats->next_stage]);
			if (game_stats->next_stage == 0) {
				game_stats->kills_per_enemy.clear();
				game_stats->new_kills_per_enemy.clear();
			}
		break;
		case GAME_SCENE::OVER:
			current_scene = new SceneGameOver();
		break;
		case GAME_SCENE::SCORE:
			current_scene = new SceneScore();
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

Game::~Game() {}
