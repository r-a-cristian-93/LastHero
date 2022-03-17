#include "Scene.h"
#include <iostream>

Scene::Scene() {}

Scene::Scene(Game* g)
	:game(g)
	,frame_current(0)
	,paused(false)
	,has_ended(false)
{
	init();
}

void Scene::init() {
	ent_mgr = EntityManager(game->assets);

	gui_view.reset(sf::FloatRect(0 ,0, game->app_conf.game_w, game->app_conf.game_h));
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
			current_fade_frames[fade]--;
			unsigned char c = static_cast<size_t>(current_fade_frames[fade] * (255/fade_frames[fade]));
			game->screen_sprite.setColor({c, c, c});

			if (current_fade_frames[fade] == 0) {
				game->screen_sprite.setColor({0, 0, 0});
				game->setScene(next_scene);
				fade = FADE_NONE;
			}
		}
		break;
	}
}

void Scene::setFade(FadeType _fade) {
	fade = _fade;
}

void Scene::setNextScene(size_t scene) {
	next_scene = scene;
}

bool Scene::isFading() {
	if (fade) return true;

	return false;
}



