#ifndef S_UPDATE
#define S_UPDATE

#include <string>
#include <vector>
#include "Entity.h"
#include "EntityManager.h"

namespace SUpdate {
	void updatePosition(const EntityVec& entities, sf::FloatRect& limits);
}

#endif
