#include "SceneMainMenu.h"
#include <cmath>
#include "SUpdate.h"
#include "SDraw.h"

SceneMainMenu::SceneMainMenu(Game* g)
	:Scene(g)
	,background(nullptr)
{
	init();
}
SceneMainMenu::~SceneMainMenu() {}

void SceneMainMenu::init() {
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::N, Action::MENU_SELECT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::M, Action::MENU_SELECT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Enter, Action::MENU_SELECT);

	background = &game->assets->getSprite("main_bg");
	sf::FloatRect b = background->getLocalBounds();
	float scale_x = game->app_conf.game_w / b.width;
	float scale_y = game->app_conf.game_h / b.height;
	background->setScale(scale_x, scale_y);

	interface.add(game->assets->getWidget("main_menu"));
	game->assets->getWidget("button_play")->setColor(mod_highlight);
	game->assets->getWidget("button_exit")->setColor(mod_dark);

	game->screen_tex.setView(gui_view);
}

void SceneMainMenu::update() {
	sInterface();

	game->screen_tex.draw(*background);
	SDraw::drawInterface(&game->screen_tex, interface.getWidgets());
	frame_current++;
}


void SceneMainMenu::sInterface() {
}


void SceneMainMenu::doAction(const Action* a) {
	if (*a->type == Action::TYPE_START) {
		switch (*a->code) {
			case Action::MENU_SELECT:
				if (selection == SELECT_EXIT) {
					game->running = false;
				}
				else if (selection == SELECT_PLAY) {
					game->setScene(Game::GAME_SCENE_PLAY);
				}
			break;
			case Action::MOVE_UP:
				game->assets->getWidget("button_play")->setColor(mod_highlight);
				game->assets->getWidget("button_exit")->setColor(mod_dark);
				selection = SELECT_PLAY;
			break;
			case Action::MOVE_DOWN:
				game->assets->getWidget("button_play")->setColor(mod_dark);
				game->assets->getWidget("button_exit")->setColor(mod_highlight);
				selection = SELECT_EXIT;
			break;
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
