#ifndef APP
#define APP

#include <deque>

#include "SharedResources.h"
#include "Entity.h"
#include "EntityManager.h"
#include "ActionManager.h"
#include "SoundManager.h"
#include "Common.h"
#include "Assets.h"
#include "Scene.h"
#include "AppConfig.h"

#include "Profiler.h"

class Scene;

typedef std::map<size_t, size_t> KillsMap;

class App {
protected:
	Scene* current_scene;
	size_t next_scene;

	virtual void init() = 0;
	virtual void setScene(size_t id) = 0;

	void sSceneFade();
	void sUserInput();
	void sChangeScene();

	void handleUIEvent(sf::Event& event, WidgetVec& widgets);

public:
	bool running;

	void run();

	void setNextScene(size_t id);
	void setStyleEditor();
	void setStyleGame();

	App();
	~App();
};

#endif
