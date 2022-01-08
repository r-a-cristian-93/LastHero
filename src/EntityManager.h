#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

#include <vector>
#include <memory>
#include <map>
#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<size_t, EntityVec> EntityMap;

class EntityManager {
private:
	EntityVec entities;
	EntityVec entities_to_add;
	EntityMap entities_tagged;
	
	size_t entities_counter;
	
public:
	EntityManager();
	void update();
	std::shared_ptr<Entity> add(size_t tag);
	EntityVec getEntities();
	EntityVec getEntities(size_t tag);
};

#endif
