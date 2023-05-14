#ifndef SCENE_SETTINGS
#define SCENE_SETTINGS

#include "Scene.h"

class SceneSettings: public Scene {
	enum {
		SELECT_RESOLUTION = 0,
		SELECT_FULLSCREEN,
		SELECT_RENDER,
		SELECT_MUSIC,
		SELECT_SFX,
		SELECT_BACK,
		SELECT_COUNT
	};

	AppConfig temp_conf = *app_conf;
	std::string selected_res = "";
	std::string selected_window_style = "";
	std::string selected_render_style = "";
	std::string selected_music_vol = "";
	std::string selected_sfx_vol = "";

	int cols = 4;
	int rows = 5;	// number of settings fields

	sf::Color mod_highlight = {220, 220, 220};
	sf::Color mod_dark = {100, 100, 100};

	size_t selection = SELECT_RESOLUTION;

	void init();
	void select(size_t s);
	void selectHorizontal(size_t action_code);
	std::string to_string(sf::VideoMode& mode);
	std::string to_string(float volume);
	std::string to_string(int render_Style);


public:
	void update() override;
	void doAction(const Action& a) override;

	SceneSettings();
	~SceneSettings();
};

#endif
