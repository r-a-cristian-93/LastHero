#ifndef S_UPDATE
#define S_UPDATE

#include <string>
#include <vector>
#include "Entity.h"
#include "EntityManager.h"
#include "Enums.h"

namespace SUpdate {
	void updatePosition(const EntityVec& entities, const sf::FloatRect& limits);
}

#endif
