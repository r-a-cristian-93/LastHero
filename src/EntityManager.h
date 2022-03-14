#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

#include <vector>
#include <memory>
#include <map>
#include "Entity.h"
#include "Assets.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<size_t, EntityVec> EntityMap;

class EntityManager {
private:
	Assets* assets;

	EntityVec entities;
	EntityVec entities_to_add;
	EntityMap entities_tagged;

	size_t entities_counter;

public:
	EntityManager();
	EntityManager(Assets* _assets);
	void update();
	std::shared_ptr<Entity> add(size_t tag);
	std::shared_ptr<Entity> add(size_t tag, size_t recipe_name);
	EntityVec& getEntities();
	EntityVec& getEntities(size_t tag);
};

#endif
