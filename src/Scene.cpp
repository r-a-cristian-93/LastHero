#include "Scene.h"
#include <iostream>

Scene::Scene() {
	if (bg_music) {
		bg_music->stop();
		bg_music = nullptr;
	}
}

Scene::Scene(Game* g)
	:game(g)
	,frame_current(0)
	,paused(false)
	,has_ended(false)
	,bg_music(nullptr)
{
	init();
}

void Scene::init() {
	ent_mgr = EntityManager(game->assets);

	gui_view.reset(sf::FloatRect(0, 0, game->app_conf.game_w, game->app_conf.game_h));
}

void Scene::sFade() {
	switch (fade) {
		case FADE_IN: {
			current_fade_frames[fade]++;
			unsigned char c = static_cast<size_t>(current_fade_frames[fade] * (255/fade_frames[fade]));
			game->screen_sprite.setColor({c, c, c});

			if (current_fade_frames[fade] >= fade_frames[fade]) {
				game->screen_sprite.setColor({255, 255, 255});
				fade = FADE_NONE;
			}

		}
		break;
		case FADE_OUT: {
			if (current_fade_frames[fade] > 0) current_fade_frames[fade]--;
			unsigned char c = static_cast<size_t>(current_fade_frames[fade] * (255/fade_frames[fade]));
			game->screen_sprite.setColor({c, c, c});

			if (current_fade_frames[fade] == 0) {
				fade = FADE_NONE;
				game->screen_sprite.setColor({0, 0, 0});
				game->setNextScene(next_scene);
			}

			if (bg_music) {
				float v = 0;

				if (current_fade_frames[fade] > 0) {
					v = current_fade_frames[fade] * (100.0f/fade_frames[fade]);
				}
				bg_music->setVolume(v);
			}
		}
		break;
	}
}

void Scene::setFade(FadeType _fade, size_t frames) {
	fade = _fade;
	fade_frames[fade] = frames;
}

void Scene::setFade(FadeType _fade, size_t frames, size_t scene) {
	next_scene = scene;
	fade = _fade;
	fade_frames[fade] = frames;
}

bool Scene::isFading() {
	if (fade) return true;

	return false;
}



