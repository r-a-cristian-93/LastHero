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
}

void Scene::init() {
	ent_mgr = EntityManager(game->assets);

	gui_view.reset(sf::FloatRect(0 ,0, game->app_conf.game_w, game->app_conf.game_h));
}
