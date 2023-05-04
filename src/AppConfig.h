#ifndef APP_CONFIG
#define APP_CONFIG

#include "Common.h"

class AppConfig {
public:
	enum {
		STYLE_WINDOWED = 6,
		STYLE_FULLSCREEN = 8
	};

	std::string window_name = "LastHero";
	int window_style = STYLE_WINDOWED;
	int max_fps = 60;
	int game_w = 1024;
	int game_h = 768;
	float game_scale = 0.95;
	sf::RenderStates render_states = sf::RenderStates::Default;
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

	float music_volume = 100;
	float sfx_volume = 100;

	AppConfig();
	void read(std::string file_name);
	void write(std::string file_name);
};

#endif
