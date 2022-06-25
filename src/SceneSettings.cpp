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
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::A, Action::MOVE_LEFT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::D, Action::MOVE_RIGHT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::N, Action::MENU_SELECT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::M, Action::MENU_SELECT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Enter, Action::MENU_SELECT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Escape, Action::CHANGE_SCENE_MENU);

	// set menu buttons

	{
		Widget& key_res = game->assets.getWidget("key_res");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.2);
		pos.y = static_cast<int>(game->app_conf.game_h*0.3);
		key_res.setColor(mod_highlight);
		key_res.setPosAbs(pos);
		interface.add(key_res);
	}
	{
		Widget& val_res = game->assets.getWidget("val_res");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.7);
		pos.y = static_cast<int>(game->app_conf.game_h*0.3);
		val_res.setColor(mod_highlight);
		val_res.setPosAbs(pos);
		interface.add(val_res);
	}
	{
		Widget& arrow_left = game->assets.getWidget("arrow_left");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.5);
		pos.y = static_cast<int>(game->app_conf.game_h*0.3);
		arrow_left.setColor(mod_highlight);
		arrow_left.setPosAbs(pos);
		interface.add(arrow_left);
	}
	{
		Widget& arrow_right = game->assets.getWidget("arrow_right");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.9);
		pos.y = static_cast<int>(game->app_conf.game_h*0.3);
		arrow_right.setColor(mod_highlight);
		arrow_right.setPosAbs(pos);
		interface.add(arrow_right);
	}
	{
		Widget& key_fullscreen = game->assets.getWidget("key_fullscreen");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.2);
		pos.y = static_cast<int>(game->app_conf.game_h*0.4);
		key_fullscreen.setColor(mod_highlight);
		key_fullscreen.setPosAbs(pos);
		interface.add(key_fullscreen);
	}
	{
		Widget& val_fullscreen = game->assets.getWidget("key_fullscreen");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.7);
		pos.y = static_cast<int>(game->app_conf.game_h*0.4);
		val_fullscreen.setColor(mod_dark);
		val_fullscreen.setPosAbs(pos);
		interface.add(val_fullscreen);
	}
	{
		Widget& arrow_left = game->assets.getWidget("arrow_left");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.5);
		pos.y = static_cast<int>(game->app_conf.game_h*0.4);
		arrow_left.setColor(mod_dark);
		arrow_left.setPosAbs(pos);
		interface.add(arrow_left);
	}
	{
		Widget& arrow_right = game->assets.getWidget("arrow_right");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.9);
		pos.y = static_cast<int>(game->app_conf.game_h*0.4);
		arrow_right.setColor(mod_dark);
		arrow_right.setPosAbs(pos);
		interface.add(arrow_right);
	}
	{
		Widget& key_music_vol = game->assets.getWidget("key_music_vol");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.2);
		pos.y = static_cast<int>(game->app_conf.game_h*0.5);
		key_music_vol.setColor(mod_highlight);
		key_music_vol.setPosAbs(pos);
		interface.add(key_music_vol);
	}
	{
		Widget& val_music_vol = game->assets.getWidget("key_music_vol");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.7);
		pos.y = static_cast<int>(game->app_conf.game_h*0.5);
		val_music_vol.setColor(mod_dark);
		val_music_vol.setPosAbs(pos);
		interface.add(val_music_vol);
	}
	{
		Widget& arrow_left = game->assets.getWidget("arrow_left");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.5);
		pos.y = static_cast<int>(game->app_conf.game_h*0.5);
		arrow_left.setColor(mod_dark);
		arrow_left.setPosAbs(pos);
		interface.add(arrow_left);
	}
	{
		Widget& arrow_right = game->assets.getWidget("arrow_right");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.9);
		pos.y = static_cast<int>(game->app_conf.game_h*0.5);
		arrow_right.setColor(mod_dark);
		arrow_right.setPosAbs(pos);
		interface.add(arrow_right);
	}
	{
		Widget& key_sfx_vol = game->assets.getWidget("key_sfx_vol");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.2);
		pos.y = static_cast<int>(game->app_conf.game_h*0.6);
		key_sfx_vol.setColor(mod_highlight);
		key_sfx_vol.setPosAbs(pos);
		interface.add(key_sfx_vol);
	}
	{
		Widget& val_sfx_vol = game->assets.getWidget("key_sfx_vol");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.7);
		pos.y = static_cast<int>(game->app_conf.game_h*0.6);
		val_sfx_vol.setColor(mod_dark);
		val_sfx_vol.setPosAbs(pos);
		interface.add(val_sfx_vol);
	}
	{
		Widget& arrow_left = game->assets.getWidget("arrow_left");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.5);
		pos.y = static_cast<int>(game->app_conf.game_h*0.6);
		arrow_left.setColor(mod_dark);
		arrow_left.setPosAbs(pos);
		interface.add(arrow_left);
	}
	{
		Widget& arrow_right = game->assets.getWidget("arrow_right");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.9);
		pos.y = static_cast<int>(game->app_conf.game_h*0.6);
		arrow_right.setColor(mod_dark);
		arrow_right.setPosAbs(pos);
		interface.add(arrow_right);
	}
	{
		Widget& button_apply = game->assets.getWidget("button_apply");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.5);
		pos.y = static_cast<int>(game->app_conf.game_h*0.75);
		button_apply.setColor(mod_dark);
		button_apply.setPosAbs(pos);
		interface.add(button_apply);
	}
	{
		Widget& button_back = game->assets.getWidget("button_back");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.5);
		pos.y = static_cast<int>(game->app_conf.game_h*0.85);
		button_back.setColor(mod_dark);
		button_back.setPosAbs(pos);
		interface.add(button_back);
	}
	{
		Widget& settings_title = game->assets.getWidget("settings_title");
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.game_w*0.5);
		pos.y = static_cast<int>(game->app_conf.game_h*0.1);
		settings_title.setColor(mod_highlight);
		settings_title.setPosAbs(pos);
		interface.add(settings_title);
	}

	selected_res = to_string(temp_conf.modes[temp_conf.current_mode_id]);

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

					if (selection == SELECT_APPLY) {
						game->applySettings(temp_conf);
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
			case Action::MOVE_LEFT:
			case Action::MOVE_RIGHT:
				if (getCurrentFade() != FADE::OUT) {
					selectHorizontal(*a->code);
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

void SceneSettings::selectHorizontal(size_t action_code) {
	switch (selection) {
		case SELECT_RESOLUTION:
			if (action_code == Action::MOVE_LEFT) {
				if (temp_conf.current_mode_id > 0) {
					temp_conf.current_mode_id--;
					selected_res = to_string(temp_conf.modes[temp_conf.current_mode_id]);
				}
			}
			else if (action_code == Action::MOVE_RIGHT) {
				if (temp_conf.current_mode_id < temp_conf.modes.size() - 1) {
					temp_conf.current_mode_id++;
					selected_res = to_string(temp_conf.modes[temp_conf.current_mode_id]);
				}
			}
		break;
	}
}

void SceneSettings::select(size_t s) {
	selection = s;

	for (size_t i = 0; i<rows + 2; i++) {		// + 2 for APPLY and BACK buttons
		if (i == selection) {
			if (i<cols) {
				interface.getWidgets()[i*4+1].setColor(mod_highlight);
				interface.getWidgets()[i*4+2].setColor(mod_highlight);
				interface.getWidgets()[i*4+3].setColor(mod_highlight);
			}
			else {
				interface.getWidgets()[cols*(rows-1)+i].setColor(mod_highlight);
			}
		}
		else {
			if (i<cols) {
				interface.getWidgets()[i*4+1].setColor(mod_dark);
				interface.getWidgets()[i*4+2].setColor(mod_dark);
				interface.getWidgets()[i*4+3].setColor(mod_dark);
			}
			else {
				interface.getWidgets()[cols*(rows-1)+i].setColor(mod_dark);
			}
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
