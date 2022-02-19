#ifndef COMMON
#define COMMON

#include <string>
#include <SFML/Graphics.hpp>

#define PI 3.14159265

struct AppConfig {
	std::string window_name;
	int window_w, window_h;
	int game_w, game_h;
	int max_fps;
	int window_style;
	int font_size;
	int font_r, font_g, font_b;
	float cam_speed, cam_treshold;
	int directions = 8;
};

#endif
