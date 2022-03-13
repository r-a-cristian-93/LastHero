#ifndef SCENE_MAIN_MENU
#define SCENE_MAIN_MENU

#include "Scene.h"
#include "Interface.h"

class SceneMainMenu: public Scene {
	Interface interface;
	sf::View gui_view;

	void init();

public:
	void update() override;
	void doAction(const Action* a) override;

	void sInterface();

	SceneMainMenu();
	SceneMainMenu(Game* g);
	~SceneMainMenu();
};

#endif
