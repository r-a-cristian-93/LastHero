#ifndef SCENE_MAIN_MENU
#define SCENE_MAIN_MENU

#include "Scene.h"
#include "Interface.h"

class SceneMainMenu: public Scene {
	sf::Sprite* background;
	Interface interface;
	std::vector<Widget*> buttons;

	sf::Color mod_highlight = {255, 255, 255};
	sf::Color mod_dark = {100, 100, 100};

	enum {
		SELECT_PLAY,
		SELECT_EXIT,
	};

	size_t selection = SELECT_PLAY;

	void init();
	void setHighlight();

public:
	void update() override;
	void doAction(const Action* a) override;

	SceneMainMenu();
	SceneMainMenu(Game* g);
	~SceneMainMenu();
};

#endif
