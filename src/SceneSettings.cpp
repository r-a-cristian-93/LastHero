#include <cmath>
#include "SceneSettings.h"

SceneSettings::SceneSettings()
	:Scene(GAME_SCENE::SETTINGS)
{
	init();
}
SceneSettings::~SceneSettings() {}

void SceneSettings::init() {
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::A, Action::MOVE_LEFT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::D, Action::MOVE_RIGHT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::N, Action::MENU_SELECT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::M, Action::MENU_SELECT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Enter, Action::MENU_SELECT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Escape, Action::CHANGE_SCENE_MENU);

	// set menu buttons

	{
		Widget& key_res = assets->getWidget("key_res");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.2);
		pos.y = static_cast<int>(app_conf->game_h*0.3);
		key_res.setColor(mod_highlight);
		key_res.setPosAbs(pos);
		interface.add(key_res);
	}
	{
		Widget& val_res = assets->getWidget("val_res");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.7);
		pos.y = static_cast<int>(app_conf->game_h*0.3);
		val_res.setColor(mod_highlight);
		val_res.setPosAbs(pos);
		interface.add(val_res);
	}
	{
		Widget& arrow_left = assets->getWidget("arrow_left");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.5);
		pos.y = static_cast<int>(app_conf->game_h*0.3);
		arrow_left.setColor(mod_highlight);
		arrow_left.setPosAbs(pos);
		interface.add(arrow_left);
	}
	{
		Widget& arrow_right = assets->getWidget("arrow_right");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.9);
		pos.y = static_cast<int>(app_conf->game_h*0.3);
		arrow_right.setColor(mod_highlight);
		arrow_right.setPosAbs(pos);
		interface.add(arrow_right);
	}
	{
		Widget& key_fullscreen = assets->getWidget("key_fullscreen");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.2);
		pos.y = static_cast<int>(app_conf->game_h*0.4);
		key_fullscreen.setColor(mod_highlight);
		key_fullscreen.setPosAbs(pos);
		interface.add(key_fullscreen);
	}
	{
		Widget& val_fullscreen = assets->getWidget("val_fullscreen");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.7);
		pos.y = static_cast<int>(app_conf->game_h*0.4);
		val_fullscreen.setColor(mod_dark);
		val_fullscreen.setPosAbs(pos);
		interface.add(val_fullscreen);
	}
	{
		Widget& arrow_left = assets->getWidget("arrow_left");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.5);
		pos.y = static_cast<int>(app_conf->game_h*0.4);
		arrow_left.setColor(mod_dark);
		arrow_left.setPosAbs(pos);
		interface.add(arrow_left);
	}
	{
		Widget& arrow_right = assets->getWidget("arrow_right");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.9);
		pos.y = static_cast<int>(app_conf->game_h*0.4);
		arrow_right.setColor(mod_dark);
		arrow_right.setPosAbs(pos);
		interface.add(arrow_right);
	}
	{
		Widget& key_render = assets->getWidget("key_render_style");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.2);
		pos.y = static_cast<int>(app_conf->game_h*0.5);
		key_render.setColor(mod_highlight);
		key_render.setPosAbs(pos);
		interface.add(key_render);
	}
	{
		Widget& val_render = assets->getWidget("val_render_style");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.7);
		pos.y = static_cast<int>(app_conf->game_h*0.5);
		val_render.setColor(mod_dark);
		val_render.setPosAbs(pos);
		interface.add(val_render);
	}
	{
		Widget& arrow_left = assets->getWidget("arrow_left");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.5);
		pos.y = static_cast<int>(app_conf->game_h*0.5);
		arrow_left.setColor(mod_dark);
		arrow_left.setPosAbs(pos);
		interface.add(arrow_left);
	}
	{
		Widget& arrow_right = assets->getWidget("arrow_right");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.9);
		pos.y = static_cast<int>(app_conf->game_h*0.5);
		arrow_right.setColor(mod_dark);
		arrow_right.setPosAbs(pos);
		interface.add(arrow_right);
	}
	{
		Widget& key_music_vol = assets->getWidget("key_music_vol");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.2);
		pos.y = static_cast<int>(app_conf->game_h*0.6);
		key_music_vol.setColor(mod_highlight);
		key_music_vol.setPosAbs(pos);
		interface.add(key_music_vol);
	}
	{
		Widget& val_music_vol = assets->getWidget("val_music_vol");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.7);
		pos.y = static_cast<int>(app_conf->game_h*0.6);
		val_music_vol.setColor(mod_dark);
		val_music_vol.setPosAbs(pos);
		interface.add(val_music_vol);
	}
	{
		Widget& arrow_left = assets->getWidget("arrow_left");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.5);
		pos.y = static_cast<int>(app_conf->game_h*0.6);
		arrow_left.setColor(mod_dark);
		arrow_left.setPosAbs(pos);
		interface.add(arrow_left);
	}
	{
		Widget& arrow_right = assets->getWidget("arrow_right");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.9);
		pos.y = static_cast<int>(app_conf->game_h*0.6);
		arrow_right.setColor(mod_dark);
		arrow_right.setPosAbs(pos);
		interface.add(arrow_right);
	}
	{
		Widget& key_sfx_vol = assets->getWidget("key_sfx_vol");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.2);
		pos.y = static_cast<int>(app_conf->game_h*0.7);
		key_sfx_vol.setColor(mod_highlight);
		key_sfx_vol.setPosAbs(pos);
		interface.add(key_sfx_vol);
	}
	{
		Widget& val_sfx_vol = assets->getWidget("val_sfx_vol");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.7);
		pos.y = static_cast<int>(app_conf->game_h*0.7);
		val_sfx_vol.setColor(mod_dark);
		val_sfx_vol.setPosAbs(pos);
		interface.add(val_sfx_vol);
	}
	{
		Widget& arrow_left = assets->getWidget("arrow_left");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.5);
		pos.y = static_cast<int>(app_conf->game_h*0.7);
		arrow_left.setColor(mod_dark);
		arrow_left.setPosAbs(pos);
		interface.add(arrow_left);
	}
	{
		Widget& arrow_right = assets->getWidget("arrow_right");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.9);
		pos.y = static_cast<int>(app_conf->game_h*0.7);
		arrow_right.setColor(mod_dark);
		arrow_right.setPosAbs(pos);
		interface.add(arrow_right);
	}
	{
		Widget& button_back = assets->getWidget("button_back");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.5);
		pos.y = static_cast<int>(app_conf->game_h*0.85);
		button_back.setColor(mod_dark);
		button_back.setPosAbs(pos);
		interface.add(button_back);
	}
	{
		Widget& settings_title = assets->getWidget("settings_title");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.5);
		pos.y = static_cast<int>(app_conf->game_h*0.1);
		settings_title.setColor(mod_highlight);
		settings_title.setPosAbs(pos);
		interface.add(settings_title);
	}

	selected_res = to_string(temp_conf.modes[temp_conf.current_mode_id]);

	if (temp_conf.window_style == AppConfig::STYLE_WINDOWED)
		selected_window_style = "OFF";
	else if (temp_conf.window_style == AppConfig::STYLE_FULLSCREEN)
		selected_window_style = "ON";

	selected_render_style = to_string(temp_conf.render_style);
	selected_sfx_vol = to_string(temp_conf.sfx_volume);
	selected_music_vol = to_string(temp_conf.music_volume);

	std::variant<std::monostate, int*, std::string*> links[static_cast<int>(Link::Target::COUNT)];
	links[static_cast<int>(Link::Target::WINDOW_RESOLUTION)] = &selected_res;
	links[static_cast<int>(Link::Target::WINDOW_STYLE)] = &selected_window_style;
	links[static_cast<int>(Link::Target::RENDER_STYLE)] = & selected_render_style;
	links[static_cast<int>(Link::Target::MUSIC_VOLUME)] = &selected_music_vol;
	links[static_cast<int>(Link::Target::SFX_VOLUME)] = &selected_sfx_vol;
	interface.setLinks(links);

	screen_tex->setView(gui_view);
}

void SceneSettings::update() {
	interface.update();

	sDrawInterface();

	frame_current += frame_mgr->getIncrement();
}

void SceneSettings::doAction(const Action& a) {
	if (*a.type == Action::TYPE_START) {
		switch (*a.code) {
			case Action::MENU_SELECT:
				if (getCurrentFade() != FADE::OUT) {
					snd_mgr->playSound("menu_confirm");
					if (selection == SELECT_BACK) {
						setFade(FADE::OUT, GAME_SCENE::MENU);
					}
					else if (selection == SELECT_RESOLUTION || selection == SELECT_FULLSCREEN || selection == SELECT_RENDER) {
						if (app_conf->window_style != temp_conf.window_style ||
							app_conf->current_mode_id != temp_conf.current_mode_id ||
							app_conf->render_style != temp_conf.render_style)
						{
							cfg_mgr->applySettings(temp_conf);
							temp_conf.write("user.cfg");
						}
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
					selectHorizontal(*a.code);
				}
			break;
			case Action::CHANGE_SCENE_MENU:
				if (getCurrentFade() != FADE::OUT) {
					snd_mgr->playSound("menu_cancel");
					setFade(FADE::OUT, GAME_SCENE::MENU);
				}
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

void SceneSettings::selectHorizontal(size_t action_code) {
	switch (selection) {
		case SELECT_RESOLUTION:
			if (action_code == Action::MOVE_LEFT) {
				if (temp_conf.current_mode_id > 0) {
					temp_conf.current_mode_id--;
				}
			}
			else if (action_code == Action::MOVE_RIGHT) {
				if (temp_conf.current_mode_id < temp_conf.modes.size() - 1) {
					temp_conf.current_mode_id++;
				}
			}

			selected_res = to_string(temp_conf.modes[temp_conf.current_mode_id]);
		break;
		case SELECT_FULLSCREEN:
			if (action_code == Action::MOVE_LEFT) {
				temp_conf.window_style = AppConfig::STYLE_FULLSCREEN;
				selected_window_style = "ON";
			}
			else if (action_code == Action::MOVE_RIGHT) {
				temp_conf.window_style = AppConfig::STYLE_WINDOWED;
				selected_window_style = "OFF";
			}
		break;
		case SELECT_RENDER:
			if (action_code == Action::MOVE_LEFT) {
				if (temp_conf.render_style > 0) {
					temp_conf.render_style--;
					selected_render_style = to_string(temp_conf.render_style);
				}
			}
			else if (action_code == Action::MOVE_RIGHT) {
				if (temp_conf.render_style < 2) {
					temp_conf.render_style++;
					selected_render_style = to_string(temp_conf.render_style);
				}
			}
		break;
		case SELECT_MUSIC:
			if (action_code == Action::MOVE_LEFT) {
				if (temp_conf.music_volume >= 5) temp_conf.music_volume -= 5;
			}
			else if (action_code == Action::MOVE_RIGHT) {
				if (temp_conf.music_volume <= 95) temp_conf.music_volume += 5;
			}

			app_conf->music_volume = temp_conf.music_volume;
			snd_mgr->setBgMusicVolume(temp_conf.music_volume);
			selected_music_vol = to_string(temp_conf.music_volume);
			temp_conf.write("user.cfg");
		break;
		case SELECT_SFX:
			if (action_code == Action::MOVE_LEFT) {
				if (temp_conf.sfx_volume >= 5) temp_conf.sfx_volume -=5;
			}
			else if (action_code == Action::MOVE_RIGHT) {
				if (temp_conf.sfx_volume <= 95) temp_conf.sfx_volume += 5;
			}

			app_conf->sfx_volume = temp_conf.sfx_volume;
			selected_sfx_vol = to_string(temp_conf.sfx_volume);
			temp_conf.write("user.cfg");
		break;
	}

	snd_mgr->playSound("menu_select");
}

void SceneSettings::select(size_t s) {
	selection = s;

	// set color for setting rows
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			if (c > 0) {
				if (r == selection) {
					interface.getWidgets()[r*cols+c].setColor(mod_highlight);
				}
				else {
					interface.getWidgets()[r*cols+c].setColor(mod_dark);
				}
			}
		}
	}

	// set color for back button
	if (selection == rows) {
		interface.getWidgets()[rows*cols].setColor(mod_highlight);
	}
	else {
		interface.getWidgets()[rows*cols].setColor(mod_dark);
	}

	snd_mgr->playSound("menu_select");
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

std::string SceneSettings::to_string(float volume) {
	return std::to_string(static_cast<int>(volume)) + " %";
}

std::string SceneSettings::to_string(int render_Style) {
	std::string str = "";
	switch (render_Style) {
		case AppConfig::RENDER_FLATSCREEN:
			str = "FLATSCREEN";
		break;
		case AppConfig::RENDER_PERFORMANCE:
			str = "PERFORMANCE";
		break;
		case AppConfig::RENDER_CLASSIC:
		default:
			str = "CLASSIC";
		break;
	}
	return str;
}
