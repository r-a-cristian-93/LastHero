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
	size_t recipe_name = assets->getRecipeName(tag);
	e = std::make_shared<Entity>(tag, entities_counter++, assets->getRecipe(tag, recipe_name));

	entities_to_add.push_back(e);
	return e;
}

std::shared_ptr<Entity> EntityManager::add(size_t tag, size_t recipe_name) {
	std::shared_ptr<Entity> e = nullptr;
	e = std::make_shared<Entity>(tag, entities_counter++, recipe_name, assets->getRecipe(tag, recipe_name));

	entities_to_add.push_back(e);
	return e;
}

void EntityManager::update() {

	//remove dead entities;
	EntityVec entities_to_keep;
	for (std::shared_ptr<Entity> e:entities) {
		bool anim_has_ended = true;

		if (e->get<CAnimation>()) {
			anim_has_ended = e->get<CAnimation>()->active_anim->hasEnded();
		}

		if (e->alive || !anim_has_ended) {
			entities_to_keep.push_back(e);
		}

	}
	entities = entities_to_keep;

	EntityMap tagged_to_keep;
	for (size_t tag=0; tag<entities_tagged.size(); tag++) {
		for (std::shared_ptr<Entity> e:entities_tagged[tag]) {
			bool anim_has_ended = true;

			if (e->get<CAnimation>()) {
				anim_has_ended = e->get<CAnimation>()->active_anim->hasEnded();
			}

			if (e->alive || !anim_has_ended) {
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

EntityVec& EntityManager::getEntities() {
	return entities;
}

EntityVec& EntityManager::getEntities(size_t tag) {
	return entities_tagged[tag];
}
