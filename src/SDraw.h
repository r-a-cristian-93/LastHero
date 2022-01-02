#ifndef S_DRAW
#define S_DRAW

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.h"
#include "EntityManager.h"

namespace SDraw {
	void drawEntities(sf::RenderWindow* w, const EntityVec& entities);	
}

#endif
