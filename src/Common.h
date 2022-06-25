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
	enum {
		STYLE_WINDOWED = 6,
		STYLE_FULLSCREEN = 8
	};

	std::string window_name = "LastHero";
	int window_style = STYLE_WINDOWED;
	int max_fps = 60;
	int game_w = 1024;
	int game_h = 768;
	float cam_speed = 15;
	float cam_treshold = 10;
	int directions = 8;
	unsigned int colmap_res = 4;
	unsigned int colmap_update = 30;
	unsigned int scene_fade_frames[FADE::COUNT];
	unsigned int score_key_frames[FRAMES_SCORE::COUNT] = {0};
	float fade_multiplier = 1;
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	size_t current_mode_id = 0;

	AppConfig() {
		std::memset(scene_fade_frames, 20, sizeof(scene_fade_frames));
	}
};

#endif
