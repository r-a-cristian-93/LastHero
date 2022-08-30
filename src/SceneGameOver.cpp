#include "SceneGameOver.h"
#include <cmath>
#include "SUpdate.h"
#include "SDraw.h"

SceneGameOver::SceneGameOver(Game* g)
	:Scene(GAME_SCENE::OVER)
	,game(g)
{
	init();
}
SceneGameOver::~SceneGameOver() {}

void SceneGameOver::init() {
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Escape, Action::MENU_SKIP);

	{
		Widget msg = Widget();
		std::string string = "";
		sf::Color color;

		if (game->stagePrev() + 1 == game->stagesCount()) {
			string = "YOU WIN";
			color = {135, 155, 70};
			snd_mgr->playBgMusic("game-win");
		}
		else {
			string = "YOU LOSE";
			color = {220, 50, 50};
			snd_mgr->playBgMusic("game-lose");
		}

		sf::Font& font = assets->getFont(Assets::FONT_PIXEL);
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.5);
		pos.y = static_cast<int>(app_conf->game_h*0.5);

		msg.setText(string, font, 40);
		msg.setColor(color);
		msg.setPosAbs(pos);
		interface.add(msg);
	}

	{
		Widget& skip = assets->getWidget("menu_skip");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.2);
		pos.y = static_cast<int>(app_conf->game_h*0.95);
		skip.setPosAbs(pos);
		interface.add(skip);
	}

	screen_tex->setView(gui_view);
}

void SceneGameOver::update() {
	SDraw::drawInterface(&*screen_tex, interface.getWidgets());

	if ((frame_current == 240 || skip_key_frames) && getCurrentFade() != FADE::OUT) {
		setFade(FADE::OUT, GAME_SCENE::SCORE);
	}

	frame_current++;
}

void SceneGameOver::doAction(const Action& a) {
	if (*a.type == Action::TYPE_START) {
		switch (*a.code) {
			case Action::MENU_SKIP:
				skip_key_frames = true;
			break;
			default:
			break;
		}
	}
	if (*a.type == Action::TYPE_END) {
		switch (*a.code) {
			default:
			break;
		}
	}
}
