#include "ConfigManager.h"
#include "SharedResources.h"
#include <SFML/Graphics.hpp>

void ConfigManager::applySettings(AppConfig& conf) {
	*app_conf = conf;

	{
		float scale = static_cast<float>(app_conf->modes[app_conf->current_mode_id].height) / app_conf->game_h;
		scale *= app_conf->game_scale;
		screen_sprite->setScale(scale,scale);

		float offset_x = (app_conf->modes[app_conf->current_mode_id].width - screen_sprite->getGlobalBounds().width)/2;
		float offset_y = (app_conf->modes[app_conf->current_mode_id].height - screen_sprite->getGlobalBounds().height)/2;

		screen_sprite->setPosition(offset_x, offset_y);
	}

	app_conf->current_render_state = &assets->getRenderStates(app_conf->render_style);

	window->create(app_conf->modes[app_conf->current_mode_id], app_conf->window_name, app_conf->window_style);
	window->setFramerateLimit(app_conf->max_fps);
	window->setKeyRepeatEnabled(false);
	window->setMouseCursorVisible(false);
}
