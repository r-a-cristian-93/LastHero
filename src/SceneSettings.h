#ifndef SCENE_SETTINGS
#define SCENE_SETTINGS

#include "Scene.h"
#include "Interface.h"

class SceneSettings: public Scene {
	enum {
		SELECT_RESOLUTION = 0,
		SELECT_FULLSCREEN,
		SELECT_MUSIC,
		SELECT_SFX,
		SELECT_BACK,
		SELECT_COUNT
	};

	AppConfig temp_conf = game->app_conf;
	std::string selected_res = "";
	std::string selected_style = "";
	std::string selected_music_vol = "";
	std::string selected_sfx_vol = "";

	Interface interface;
	int cols = 4;
	int rows = 4;	// number of settings fields

	sf::Color mod_highlight = {220, 220, 220};
	sf::Color mod_dark = {100, 100, 100};

	size_t selection = SELECT_RESOLUTION;

	void init();
	void select(size_t s);
	void selectHorizontal(size_t action_code);
	std::string to_string(sf::VideoMode& mode);
	std::string to_string(float volume);

public:
	void update() override;
	void doAction(const Action* a) override;

	SceneSettings();
	SceneSettings(Game* g);
	~SceneSettings();
};

#endif
