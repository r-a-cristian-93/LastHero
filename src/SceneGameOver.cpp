#include "SceneGameOver.h"
#include <cmath>
#include "SUpdate.h"
#include "SDraw.h"

SceneGameOver::SceneGameOver(Game* g)
	:Scene(g, GAME_SCENE::OVER)
{
	init();
}
SceneGameOver::~SceneGameOver() {}

void SceneGameOver::init() {
	{
		Widget msg = Widget();
		std::string string = "";
		sf::Color color;

		if (game->stagePrev() + 1 == game->stagesCount()) {
			string = "YOU WIN";
			color = {135, 155, 70};
			game->snd_mgr.playBgMusic("game-win");
		}
		else {
			string = "YOU LOSE";
			color = {220, 50, 50};
			game->snd_mgr.playBgMusic("game-lose");
		}

		sf::Font& font = game->assets.getFont(Assets::FONT_PIXEL);
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.5);
		pos.y = static_cast<int>(game->app_conf.game_h*0.5);

		msg.setText(string, font, 40);
		msg.setColor(color);
		msg.setPosAbs(pos);
		interface.add(msg);
	}

	game->screen_tex.setView(gui_view);
}

void SceneGameOver::update() {
	SDraw::drawInterface(&game->screen_tex, interface.getWidgets());

	if (frame_current == 240) {
		setFade(FADE::OUT, 60, GAME_SCENE::SCORE);
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
