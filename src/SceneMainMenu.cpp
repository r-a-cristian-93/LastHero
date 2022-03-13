#include "SceneMainMenu.h"
#include <cmath>
#include "SUpdate.h"
#include "SDraw.h"

SceneMainMenu::SceneMainMenu(Game* g)
	:Scene(g)
{
	init();
}
SceneMainMenu::~SceneMainMenu() {}

void SceneMainMenu::init() {
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);

	interface.add(game->assets->getWidget("player_health"));
	interface.add(game->assets->getWidget("base_health"));
	interface.add(game->assets->getWidget("total_kills"));

	gui_view.reset(sf::FloatRect(0 ,0, game->app_conf.game_w, game->app_conf.game_h));
	game->screen_tex.setView(gui_view);
}

void SceneMainMenu::update() {
	sInterface();
	SDraw::drawInterface(&game->screen_tex, interface.getWidgets());
	frame_current++;
}


void SceneMainMenu::sInterface() {
	static_cast<WidgetText*>(game->assets->getWidget("player_health_text"))->updateText();
	static_cast<WidgetText*>(game->assets->getWidget("base_health_text"))->updateText();
	static_cast<WidgetText*>(game->assets->getWidget("total_kills_text"))->updateText();
}


void SceneMainMenu::doAction(const Action* a) {
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
