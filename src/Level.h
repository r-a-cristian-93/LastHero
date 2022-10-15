#ifndef LEVEL_H
#define LEVEL_H

#include "Common.h"
#include "Tilemap.h"
#include "Action.h"
#include "Assets.h"

class Level {
public:
	std::string path = "";
	sf::Vector2u tile_size;
	sf::Vector2u map_size;
	Tilemap map_ground;
	std::string bg_music;
	std::vector<Action> actions;

	Level();
	Level(std::string path);
};


#endif
