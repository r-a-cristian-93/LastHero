#ifndef SYSTEM_VIEW_H
#define SYSTEM_VIEW_H

#include <math.h>
#include "System.h"

float squareDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}

class SystemView: public System {
public:
	using System::System;

	void operator() () {
	//update camera position
		play_data.cam.target = play_data.player->get<CTransform>()->pos;
		float square_delta = squareDistance(play_data.cam.pos, play_data.cam.target);

		if (square_delta > app_conf->cam_treshold) {
			play_data.cam.pos += ((play_data.cam.target - play_data.cam.pos) / app_conf->cam_speed);
		}

		//update view position
		int w = app_conf->game_w;
		int h = app_conf->game_h;
		sf::FloatRect world = play_data.level.map_ground.getBounds();
		sf::FloatRect rect(play_data.cam.pos.x-w/2, play_data.cam.pos.y-h/2, w, h);

		//fix weird lines between map tiles when moving
		rect.left = round(rect.left);
		rect.top = round(rect.top);

		if (rect.left < 0) rect.left = 0;
		if (rect.top < 0) rect.top = 0;
		if (rect.left + rect.width > world.width) rect.left = world.width - w;
		if (rect.top + rect.height > world.height) rect.top = world.height - h;

		game_view->reset(rect);
	}

};

#endif