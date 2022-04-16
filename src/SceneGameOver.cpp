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

	{
		Widget msg = Widget();
		std::string string = "";
		sf::Color color;

		if (game->stagePrev() + 1 == game->stagesCount()) {
			string = "YOU WIN";
			color = {135, 155, 70};
		}
		else {
			string = "YOU LOSE";
			color = {255, 50, 50};
		}

		sf::Font& font = game->assets.getFont(Assets::FONT_COURIER);
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.window_w*0.5);
		pos.y = static_cast<int>(game->app_conf.window_h*0.5);

		msg.setText(string, font, 150);
		msg.setColor(color);
		msg.setPosAbs(pos);
		interface.add(msg);
	}

	game->screen_tex.setView(gui_view);
}

void SceneGameOver::update() {
	SDraw::drawInterface(&game->screen_tex, interface.getWidgets());

	if (frame_current == 120) {
		setFade(FADE_OUT, 60, Game::GAME_SCENE_SCORE);
	}

	frame_current++;
	sFade();
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
