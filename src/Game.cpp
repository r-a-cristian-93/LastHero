#include <iostream>

#include "Game.h"
#include "ScenePlay.h"
#include "SceneMainMenu.h"
#include "SceneGameOver.h"
#include "SceneScore.h"
#include "SceneSettings.h"
#include "Profiler.h"

Game::Game()
	:running(true)
	,current_scene(nullptr)
	,next_scene(0)
{
	init();
}

void Game::run() {
	while(running) {
		PROFILE_FUNCTION();

		if (window->isOpen()) {
			frame_mgr->update();

			sUserInput();

			screen_tex->clear();

			if (current_scene) {
				current_scene->update();
				sSceneFade();
			}

			screen_tex->display();

			window->draw(*screen_sprite, *app_conf->current_render_state);
			window->display();

			sChangeScene();
		}
	}
}

void Game::sUserInput() {
	sf::Event event;
	Action::Code action_code = Action::NONE;

	while (window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			running = false;
		}
		else if (event.type == sf::Event::KeyPressed) {
			action_code = act_mgr->getCode(ActionManager::DEV_KEYBOARD, event.key.code);

			if (action_code) {
				Action action(action_code, Action::TYPE_START);
				current_scene->doAction(action);
			}
		}
		else if (event.type == sf::Event::KeyReleased) {
			action_code = act_mgr->getCode(ActionManager::DEV_KEYBOARD, event.key.code);

			if (action_code) {
				Action action(action_code, Action::TYPE_END);
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

void Game::sSceneFade() {
	FadeType& fade = current_scene->fade;
	float * current_fade_frames = current_scene->current_fade_frames;
	float* fade_frames = current_scene->fade_frames;
	bool& music_fade_out = current_scene->music_fade_out;
	float frame_increment = frame_mgr->getIncrement();

	switch (fade) {
		case FADE::IN: {
			current_fade_frames[fade] += frame_increment;
			unsigned char c = current_fade_frames[fade] * (255/fade_frames[fade]);
			screen_sprite->setColor({c, c, c});

			if (current_fade_frames[fade] >= fade_frames[fade]) {
				screen_sprite->setColor({255, 255, 255});
				fade = FADE::NONE;
			}
		}
		break;
		case FADE::OUT: {
			if (current_fade_frames[fade] > 0) {
				current_fade_frames[fade] -= frame_increment;
			}
			unsigned char c = current_fade_frames[fade] * (255/fade_frames[fade]);

			if (music_fade_out && current_scene->next_scene != GAME_SCENE::SETTINGS) {
				float v = 0;

				if (current_fade_frames[fade] > 0) {
					v = current_fade_frames[fade] * (app_conf->music_volume/fade_frames[fade]);
				}
				snd_mgr->setBgMusicVolume(v);
			}

			screen_sprite->setColor({c, c, c});

			if (current_fade_frames[fade] > -frame_increment && current_fade_frames[fade] < frame_increment) {
				fade = FADE::NONE;
				screen_sprite->setColor({0, 0, 0});
				setNextScene(current_scene->next_scene);
			}
		}
		break;
	}
}


void Game::setNextScene(size_t id) {
	next_scene = id;
}

void Game::init() {
	//load texture after creating the window causes sementation fault;
	screen_tex->create(app_conf->game_w, app_conf->game_h);
	screen_sprite->setTextureRect({0, 0, app_conf->game_w, app_conf->game_h});

	cfg_mgr->applySettings(*app_conf);

	setScene(GAME_SCENE::MENU);
}

void Game::setScene(size_t id) {
	delete current_scene;
	current_scene = nullptr;

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
			running = false;
		break;
		default:
			std::cout << __FILE__ << ":" << __LINE__ << " ERROR : Scene id " << id << " not handled\n";
		break;
	}
}

Game::~Game() {
	delete current_scene;
}

