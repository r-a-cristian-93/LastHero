#ifndef SCENE
#define SCENE

#include "Game.h"
#include "EntityManager.h"
#include "ActionManager.h"
#include "Action.h"

class Scene {
protected:
	Game* game;
	EntityManager ent_mgr;
	ActionManager act_mgr;

	int frame_current;
	bool paused, has_ended;

	virtual void update() = 0;
	virtual void doAction(const Action* a) = 0;
	//void sRender() = 0;

	void simulate(int i) ;
	void init();

public:
	Scene();
	Scene(Game* g);
};

#endif
