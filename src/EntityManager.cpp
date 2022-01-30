#include "EntityManager.h"
#include <iostream>

EntityManager::EntityManager()
	:entities_counter(0)
	{}

EntityManager::EntityManager(Assets* _assets)
	:entities_counter(0)
	,assets(_assets)
	{}

std::shared_ptr<Entity> EntityManager::add(size_t tag) {
	std::shared_ptr<Entity> e = nullptr;
	if (tag == Entity::TAG_PLAYER) {
		e = std::make_shared<Entity>(tag, entities_counter++, assets->getRecipePlayer());
	}
	else if (tag == Entity::TAG_ENEMY) {
		e = std::make_shared<Entity>(tag, entities_counter++, assets->getRecipeEnemyRand());
	}
	else if (tag == Entity::TAG_BULLET) {
		e = std::make_shared<Entity>(tag, entities_counter++, assets->getRecipeBullet());
	}
	else if (tag == Entity::TAG_MISSLE) {
		e = std::make_shared<Entity>(tag, entities_counter++, assets->getRecipeMissle());
	}
	else if (tag == Entity::TAG_CHILD) {
		e = std::make_shared<Entity>(tag, entities_counter++);
	}

	entities_to_add.push_back(e);
	return e;
}

std::shared_ptr<Entity> EntityManager::add(size_t tag, std::string& recipe_name) {
	std::shared_ptr<Entity> e = nullptr;
	if (tag == Entity::TAG_ENEMY) {
		e = std::make_shared<Entity>(tag, entities_counter++, assets->getRecipeEnemy(recipe_name));
	}

	entities_to_add.push_back(e);
	return e;
}

void EntityManager::update() {

	//remove dead entities;
	EntityVec entities_to_keep;
	for (std::shared_ptr<Entity> e:entities) {
		if (e->alive) {
			entities_to_keep.push_back(e);
		}
	}
	entities = entities_to_keep;

	EntityMap tagged_to_keep;
	for (size_t tag=0; tag<entities_tagged.size(); tag++) {
		for (std::shared_ptr<Entity> e:entities_tagged[tag]) {
			if (e->alive) {
				tagged_to_keep[e->tag].push_back(e);
			}
		}
	}
	entities_tagged = tagged_to_keep;

	// add new entities
	for (std::shared_ptr<Entity> e:entities_to_add) {
		entities.push_back(e);
		entities_tagged[e->tag].push_back(e);
	}
	entities_to_add.clear();
}

EntityVec EntityManager::getEntities() {
	return entities;
}

EntityVec EntityManager::getEntities(size_t tag) {
	return entities_tagged[tag];
}
