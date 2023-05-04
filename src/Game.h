#ifndef GAME
#define GAME

#include "Scene.h"

class Game {
private:
	Scene* current_scene;
	size_t next_scene;
	sf::RenderStates render_states;

	void init();
	void setScene(size_t id);
	void setNextScene(size_t id);


	void sSceneFade();
	void sUserInput();
	void sChangeScene();

public:
	bool running;

	void run();

	Game();
	~Game();
};

#endif
