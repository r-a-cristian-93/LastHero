#include <fstream>
#include <iostream>
#include <cmath>

#include "Game.h"
#include "ScenePlay.h"
#include "SceneMainMenu.h"
#include "SceneGameOver.h"
#include "SceneScore.h"
#include "SceneSettings.h"
#include "SceneEditor.h"

Game::Game()
{
	init();
}

void Game::init() {
	//load texture after creating the window causes sementation fault;
	screen_tex->create(app_conf->game_w, app_conf->game_h);
	screen_sprite->setTextureRect({0, 0, app_conf->game_w, app_conf->game_h});

	cfg_mgr->applySettings(*app_conf);

	setScene(GAME_SCENE::MENU);

	running = true;
}

void Game::setScene(size_t id) {
	if (current_scene) {
		if (current_scene->scene_type == GAME_SCENE::EDITOR) setStyleGame();
	}

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
		case GAME_SCENE::EDITOR:
			current_scene = new SceneEditor(assets->getStages()[game_stats->next_stage]);
			setStyleEditor();
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
