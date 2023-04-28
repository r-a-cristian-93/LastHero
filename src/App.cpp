#include <iostream>

#include "App.h"
#include "Profiler.h"

App::App()
	:running(true)
	,current_scene(nullptr)
	,render_states(sf::RenderStates::Default)
	,next_scene(0)
{}

void App::run() {
	sf::Event event;

	while(running) {
		PROFILE_SCOPE("MAIN_GAME_LOOP");

		if (window->isOpen()) {
			sUserInput();

			screen_tex->clear();

			if (current_scene) {
				current_scene->update();
				sSceneFade();
			}

			screen_tex->display();

			window->draw(*screen_sprite, render_states);
			window->display();

			sChangeScene();
		}
	}
}

void App::sUserInput() {
	PROFILE_FUNCTION();

	sf::Event event;
	int action_code = Action::NONE;

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
		else if (event.type == sf::Event::MouseButtonPressed) {
			handleUIEvent(event, current_scene->interface.getWidgets());

			action_code = act_mgr->getCode(ActionManager::DEV_MOUSE, event.mouseButton.button);

			if (action_code != 0) {
				Action action(action_code, Action::TYPE_START, sf::Mouse::getPosition());
				current_scene->doAction(action);
			}
		}
	}
}

void App::handleUIEvent(sf::Event& event, WidgetVec& widgets) {
	sf::Vector2i m_pos(sf::Mouse::getPosition(*window));

	for (Widget& w: widgets) {
		sf::IntRect rect (w.getGlobalBounds());

		if (w.on_click && rect.contains(m_pos)) {
			Action action(w.on_click, Action::TYPE_START);
			current_scene->doAction(action);
		}
		handleUIEvent(event, w.getChilds());
	}
}

void App::sChangeScene() {
	if (next_scene) {
		setScene(next_scene);
		next_scene = 0;
	}
}

void App::sSceneFade() {
	unsigned char * current_fade_frames = current_scene->current_fade_frames;
	FadeType& fade = current_scene->fade;
	const unsigned int* fade_frames = current_scene->fade_frames;
	bool& music_fade_out = current_scene->music_fade_out;

	switch (fade) {
		case FADE::IN: {
			current_fade_frames[fade]++;
			unsigned char c = current_fade_frames[fade] * (255/fade_frames[fade]);
			screen_sprite->setColor({c, c, c});

			if (current_fade_frames[fade] >= fade_frames[fade]) {
				screen_sprite->setColor({255, 255, 255});
				fade = FADE::NONE;
			}

		}
		break;
		case FADE::OUT: {
			if (current_fade_frames[fade] > 0) current_fade_frames[fade]--;
			unsigned char c = current_fade_frames[fade] * (255/fade_frames[fade]);

			if (music_fade_out && current_scene->next_scene != GAME_SCENE::SETTINGS) {
				float v = 0;

				if (current_fade_frames[fade] > 0) {
					v = current_fade_frames[fade] * (app_conf->music_volume/fade_frames[fade]);
				}
				snd_mgr->setBgMusicVolume(v);
			}

			screen_sprite->setColor({c, c, c});

			if (current_fade_frames[fade] == 0) {
				fade = FADE::NONE;
				screen_sprite->setColor({0, 0, 0});
				setNextScene(current_scene->next_scene);
			}


		}
		break;
	}
}


void App::setNextScene(size_t id) {
	next_scene = id;
}

App::~App() {
	delete current_scene;
}
