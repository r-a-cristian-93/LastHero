#include "Scene.h"

Scene::Scene() {}

Scene::Scene(Game* g)
	:game(g)
	,frame_current(0)
	,paused(false)
	,has_ended(false)
	,music_fade_in(false)
	,music_fade_out(false)
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

			if (music_fade_out) {
				float v = 0;

				if (current_fade_frames[fade] > 0) {
					v = current_fade_frames[fade] * (100.0f/fade_frames[fade]);
				}
				game->snd_mgr.setBgMusicVolume(v);
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

Scene::~Scene() {
	game->snd_mgr.stopBgMusic();
}

