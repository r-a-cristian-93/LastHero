#include "SceneGameOver.h"
#include <cmath>

SceneGameOver::SceneGameOver()
	:Scene(GAME_SCENE::OVER)
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

		if (game_stats->prev_stage + 1 == game_stats->stagesCount()) {
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
	sDrawInterface();

	if (((frame_current >= 240 && !key_frame_rendered) || skip_key_frames) && getCurrentFade() != FADE::OUT) {
		key_frame_rendered = true;
		setFade(FADE::OUT, GAME_SCENE::SCORE);
	}

	frame_current += app_conf->frame_dt * app_conf->game_speed;
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
