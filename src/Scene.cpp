#include "Scene.h"

Scene::Scene() {}

Scene::Scene(Game* g)
	:game(g)
	,frame_current(0)
	,paused(false)
	,has_ended(false)
{
	init();
}

void Scene::init() {
	ent_mgr = EntityManager();
	act_mgr = ActionManager();
}
