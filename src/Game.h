#ifndef GAME
#define GAME

#include "Scene.h"

class Game {
protected:
	Scene* current_scene;
	size_t next_scene;
	sf::RenderStates render_states;

	void init();
	void setScene(size_t id);

	void sSceneFade();
	void sUserInput();
	void sChangeScene();

public:
	bool running;

	void run();
	void setNextScene(size_t id);

	Game();
	~Game();
};

#endif
