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
	if (fade_in) {
		current_fade_in++;
		unsigned char c = static_cast<size_t>(current_fade_in * (255/fade_in_frames));
		game->screen_sprite.setColor({c, c, c});

		if (current_fade_in >= fade_in_frames) {
			game->screen_sprite.setColor({255, 255, 255});
			fade_in = false;
		}
	}

	if (fade_out) {
		current_fade_out--;
		unsigned char c = static_cast<size_t>(current_fade_out * (255/fade_out_frames));
		game->screen_sprite.setColor({c, c, c});

		if (current_fade_out == 0) {
			game->setScene(next_scene);
			game->screen_sprite.setColor({0, 0, 0});
		}
	}
}
