#ifndef COMMON
#define COMMON

//#define DEBUG_PROFILING
#define DRAW_COLLISION_BOX
//#define DEBUG_RECIPE_NAME_ID
//#define DEBUG_SPAWN_ENTITY
//#define DEBUG_ENEMIES_LEFT
#define DEBUG_COLLISION_LAYER
#define DEBUG_DIRECTION
#define DEBUG_GRID

#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Enums.h"

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
	unsigned int colmap_res = 1;
	unsigned int colmap_update = 20;
	unsigned char scene_fade_frames[GAME_SCENE::COUNT][FADE::COUNT];
	float fade_multiplier = 1;

	AppConfig() {
		std::memset(scene_fade_frames, 60, sizeof(scene_fade_frames));
	}
};

#endif
