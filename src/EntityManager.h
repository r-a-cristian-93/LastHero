#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

#include <vector>
#include <memory>
#include <map>
#include "Entity.h"
#include "Assets.h"
#include "SharedResources.h"

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
	std::shared_ptr<Entity> add(size_t tag, size_t recipe_name);
	EntityVec& getEntities();
	EntityVec& getEntities(size_t tag);
	void spawnEntity(size_t tag, size_t recipe_name, sf::Vector2f& pos, size_t state, size_t facing);
	void spawnEntity(size_t tag, size_t recipe_name, std::shared_ptr<Entity> owner, sf::Vector2f& pos, size_t state, size_t facing);

	sf::Vector2f dirOf(size_t facing);
};

#endif
