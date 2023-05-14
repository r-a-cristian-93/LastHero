#ifndef APP_CONFIG
#define APP_CONFIG

#include "Common.h"

class AppConfig {
public:
	enum {
		STYLE_WINDOWED = 6,
		STYLE_FULLSCREEN = 8
	};

	enum {
		RENDER_CLASSIC = 0,
		RENDER_FLATSCREEN,
		RENDER_PERFORMANCE
	};

	std::string window_name = "LastHero";
	int window_style = STYLE_WINDOWED;
	int render_style = RENDER_CLASSIC;
	int max_fps = 60;
	float game_speed = 60;	// normal speed at 60 fps
	int game_w = 1024;
	int game_h = 768;
	float game_scale = 0.95;
	sf::RenderStates render_states = sf::RenderStates::Default;
	float cam_speed = 15;
	float cam_treshold = 10;
	int directions = 8;
	unsigned int colmap_res = 4;
	unsigned int colmap_update = 30;
	float scene_fade_frames[FADE::COUNT];
	float score_key_frames[FRAMES_SCORE::COUNT] = {0};
	float fade_multiplier = 1;
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	size_t current_mode_id = 0;

	float music_volume = 100;
	float sfx_volume = 100;

	AppConfig();
	void read(std::string file_name);
	void write(std::string file_name);
};

#endif
