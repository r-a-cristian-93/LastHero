#ifndef COMMON
#define COMMON

//#define DEBUG_PROFILING
//#define DRAW_COLLISION_BOX
//#define DEBUG_RECIPE_NAME_ID
//#define DEBUG_SPAWN_ENTITY
//#define DEBUG_ENEMIES_LEFT
//#define DEBUG_COLLISION_LAYER
//#define DEBUG_DIRECTION
//#define DEBUG_GRID
//#define DEBUG_ENTITY_POS

#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Enums.h"
#include <iostream>

#define PI 3.14159265

struct AppConfig {
	std::string window_name;
	//int window_w, window_h;
	sf::Vector2f window_res;
	int game_w, game_h;
	int max_fps;
	int window_style;
	int font_size;
	int font_r, font_g, font_b;
	float cam_speed, cam_treshold;
	int directions = 8;
	unsigned int colmap_res = 1;
	unsigned int colmap_update = 20;
	unsigned int scene_fade_frames[FADE::COUNT];
	unsigned int score_key_frames[FRAMES_SCORE::COUNT] = {0};
	float fade_multiplier = 1;
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	sf::VideoMode* mode_current = &modes[0];

	AppConfig() {
		std::memset(scene_fade_frames, 20, sizeof(scene_fade_frames));
		for (int i = 0; i < modes.size(); i++) {
			std::cout << "Mode #" << i << "\t"
					  << modes[i].width << "x" << modes[i].height << " \t "
					  << modes[i].bitsPerPixel << " bpp" << std::endl;
		}

	}
};

#endif
