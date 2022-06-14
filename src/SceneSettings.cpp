#include "SceneSettings.h"
#include <cmath>
#include "SUpdate.h"
#include "SDraw.h"

SceneSettings::SceneSettings(Game* g)
	:Scene(g, GAME_SCENE::SETTINGS)
{
	init();
}
SceneSettings::~SceneSettings() {}

void SceneSettings::init() {
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::N, Action::MENU_SELECT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::M, Action::MENU_SELECT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Enter, Action::MENU_SELECT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Escape, Action::CHANGE_SCENE_MENU);

	// set menu buttons

	{
		Widget& exit = game->assets.getWidget("button_back");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.5);
		pos.y = static_cast<int>(game->app_conf.game_h*0.84);
		exit.setColor(mod_highlight);
		exit.setPosAbs(pos);
		interface.add(exit);
	}

	game->screen_tex.setView(gui_view);
}

void SceneSettings::update() {
	interface.update();

	SDraw::drawInterface(&game->screen_tex, interface.getWidgets());

	frame_current++;
	sFade();
}

void SceneSettings::doAction(const Action* a) {
	if (*a->type == Action::TYPE_START) {
		switch (*a->code) {
			case Action::MENU_SELECT:
				if (getCurrentFade() != FADE::OUT) {
					game->snd_mgr.playSound("menu_confirm");
					if (selection == SELECT_BACK) {
						setFade(FADE::OUT, GAME_SCENE::MENU);
					}
				}
			break;
			case Action::MOVE_UP:
				if (getCurrentFade() != FADE::OUT) {
					if (selection > 0) select(selection-1);
				}
			break;
			case Action::MOVE_DOWN:
				if (getCurrentFade() != FADE::OUT) {
					if (selection < SELECT_COUNT-1) {
						select(selection + 1);
					}
				}
			break;
			case Action::CHANGE_SCENE_MENU:
				if (getCurrentFade() != FADE::OUT) {
					setFade(FADE::OUT, GAME_SCENE::MENU);
				}
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

void SceneSettings::select(size_t s) {
	selection = s;

	for (size_t i = 0; i<interface.getWidgets().size(); i++) {
		if (i == selection) {
			interface.getWidgets()[i].setColor(mod_highlight);
		}
		else {
			interface.getWidgets()[i].setColor(mod_dark);
		}
	}

	game->snd_mgr.playSound("menu_select");
}
