#include "Scene.h"
#include <iostream>

Scene::Scene() {}

Scene::Scene(Game* g)
	:game(g)
	,frame_current(0)
	,paused(false)
	,has_ended(false)
{
	init();
	std::cout << "created Scene\n";
}

void Scene::init() {
	ent_mgr = EntityManager(game->assets);
}
