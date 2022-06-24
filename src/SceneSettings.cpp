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
		exit.setColor(mod_dark);
		exit.setPosAbs(pos);
		interface.add(exit);
	}
	{
		Widget& key_res = game->assets.getWidget("key_res");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.3);
		pos.y = static_cast<int>(game->app_conf.game_h*0.2);
		key_res.setColor(mod_highlight);
		key_res.setPosAbs(pos);
		interface.add(key_res);
	}
	{
		Widget& val_res = game->assets.getWidget("val_res");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.7);
		pos.y = static_cast<int>(game->app_conf.game_h*0.2);
		val_res.setColor(mod_highlight);
		val_res.setPosAbs(pos);
		interface.add(val_res);
	}

	selected_res = to_string(game->app_conf.modes[game->app_conf.current_mode_id]);

	std::string* links[Widget::LINK_COUNT];
	links[Widget::LINK_RESOLUTION] = &selected_res;

	interface.setLinks(links);

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

std::string SceneSettings::to_string(sf::VideoMode& mode) {
	std::string str = "";

	str = std::to_string(mode.width)
			+ "x"
			+ std::to_string(mode.height)
			+ " "
			+ std::to_string(mode.bitsPerPixel)
			+ "bpp";

	return str;
}
