#ifndef SCENE_SETTINGS
#define SCENE_SETTINGS

#include "Scene.h"
#include "Interface.h"

class SceneSettings: public Scene {
	enum {
		SELECT_BACK = 0,
		SELECT_COUNT
	};

	std::string selected_res = "RESxRES";

	Interface interface;

	sf::Color mod_highlight = {220, 220, 220};
	sf::Color mod_dark = {100, 100, 100};

	size_t selection = SELECT_BACK;

	void init();
	void select(size_t s);

public:
	void update() override;
	void doAction(const Action* a) override;

	SceneSettings();
	SceneSettings(Game* g);
	~SceneSettings();
};

#endif
