#ifndef APP
#define APP

#include "Scene.h"

class App {
protected:
	Scene* current_scene;
	size_t next_scene;
	sf::RenderStates render_states;

	virtual void init() = 0;
	virtual void setScene(size_t id) = 0;

	void sSceneFade();
	void sUserInput();
	void sChangeScene();

public:
	bool running;

	void run();
	void setNextScene(size_t id);

	App();
	~App();
};

#endif
