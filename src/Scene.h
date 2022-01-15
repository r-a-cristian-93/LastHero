#ifndef SCENE
#define SCENE

#include "Game.h"
#include "EntityManager.h"
#include "ActionManager.h"
#include "Action.h"

class Game;

class Scene {
protected:
	Game* game;
	EntityManager ent_mgr;

	int frame_current;
	bool paused, has_ended;

	void init();

public:
	virtual void update() = 0;
	virtual void doAction(const Action* a) = 0;

	Scene();
	Scene(Game* g);
	virtual ~Scene() {}
};

#endif
