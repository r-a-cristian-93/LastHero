#ifndef SCENE_PLAY_DATA_H
#define SCENE_PLAY_DATA_H

#include "MapCollision.h"
#include "Level.h"

struct ScenePlayData {
	EntityManager ent_mgr;

	std::string level_path;
	Level level;
	MapCollision collision_map;

	Camera cam;
	Widget* paused_widget = nullptr;
	int total_kills;
	KillsMap kills_per_enemy;

	std::shared_ptr<Entity> player;
	std::shared_ptr<Entity> base;

	ScenePlayData(std::string _level_path)
		:level_path(_level_path)
		,total_kills(0)
		,collision_map(ent_mgr)
		{}
};


#endif