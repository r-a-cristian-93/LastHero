#include "Scene.h"
#include "SharedResources.h"
#include "ActionManager.h"

Scene::Scene() {}

Scene::Scene(SceneType type)
	:scene_type(type)
	,frame_current(0)
	,paused(false)
	,has_ended(false)
	,music_fade_in(false)
	,music_fade_out(false)
{
	init();
}

void Scene::init() {
	fade_frames = app_conf->scene_fade_frames;

	setFade(FADE::IN);

	gui_view.reset(sf::FloatRect(0, 0, app_conf->game_w, app_conf->game_h));

	if (scene_type == GAME_SCENE::MENU && !snd_mgr->bgPlaying())
		snd_mgr->playBgMusic("intro");
}

void Scene::setFade(FadeType _fade) {
	fade = _fade;
	if (fade == FADE::OUT) current_fade_frames[fade] = fade_frames[fade];
}

void Scene::setFade(FadeType _fade, size_t scene) {
	next_scene = scene;
	fade = _fade;
	if (fade == FADE::OUT) current_fade_frames[fade] = fade_frames[fade];
}

bool Scene::isFading() {
	if (fade) return true;

	return false;
}

const FadeType Scene::getCurrentFade() {
	return fade;
}

Scene::~Scene() {
	if (next_scene != GAME_SCENE::SETTINGS && scene_type != GAME_SCENE::SETTINGS)	snd_mgr->stopBgMusic();
}

