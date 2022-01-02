#ifndef S_LOAD
#define S_LOAD

#include <vector>
#include <string>
#include "Entity.h"
#include "EntityManager.h"

namespace SLoad {
	void loadSettings(std::string file_name);
	void loadEntities(std::string file_name, EntityManager& em);
}

#endif
