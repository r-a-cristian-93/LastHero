#include "SceneGameOver.h"
#include <cmath>
#include "SUpdate.h"
#include "SDraw.h"

SceneGameOver::SceneGameOver(Game* g)
	:Scene(g)
{
	init();
}
SceneGameOver::~SceneGameOver() {}

void SceneGameOver::init() {
	setFade(FADE_IN, 60);
	interface.add(game->assets->getWidget("game_over"));
	game->screen_tex.setView(gui_view);
}

void SceneGameOver::update() {
	SDraw::drawInterface(&game->screen_tex, interface.getWidgets());

	if (frame_current == 120) {
		setFade(FADE_OUT, 60, Game::GAME_SCENE_SCORE);
	}

	sFade();
	frame_current++;
}

void SceneGameOver::doAction(const Action* a) {
	if (*a->type == Action::TYPE_START) {
		switch (*a->code) {
			default:
			break;
		}
	}
	if (*a->type == Action::TYPE_END) {
		switch (*a->code) {
			default:
			break;
		}
	}
}
