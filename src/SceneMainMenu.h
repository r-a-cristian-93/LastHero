#ifndef SCENE_MAIN_MENU
#define SCENE_MAIN_MENU

#include "Scene.h"
#include "Game.h"

class SceneMainMenu: public Scene {
	enum {
		SELECT_PLAY = 0,
		SELECT_SETTINGS,
		SELECT_EXIT,
		SELECT_COUNT
	};

	sf::Sprite* background;

	sf::Color mod_highlight = {220, 220, 220};
	sf::Color mod_dark = {100, 100, 100};

	size_t selection = SELECT_PLAY;

	void init();
	void select(size_t s);

public:
	void update() override;
	void doAction(const Action& a) override;

	SceneMainMenu();
	~SceneMainMenu();
};

#endif
