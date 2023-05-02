#include "EntityManager.h"
#include <iostream>

EntityManager::EntityManager()
	:entities_counter(0)
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

void EntityManager::spawnEntity(size_t tag, size_t recipe_name, sf::Vector2f& pos, size_t state, size_t facing) {
	spawnEntity(tag, recipe_name, nullptr, pos, state, facing);
}

void EntityManager::spawnEntity(size_t tag, size_t recipe_name, std::shared_ptr<Entity> owner, sf::Vector2f& pos, size_t state, size_t facing) {

#ifdef DEBUG_SPAWN_ENTITY
	std::cout << "spawnEntity()  tag: " << tag << " name_id: " << recipe_name << " state: " << state << " facing: " << facing << std::endl;
#endif

	sf::Vector2f dir = dirOf(facing);

	std::shared_ptr<Entity> e = add(tag, recipe_name);

	if (e) {
		if (state == Entity::STATE_SPAWN) e->blocked = true;
		e->owner = owner;
		e->state = state;
		e->facing = facing;

		e->get<CTransform>()->pos = pos;
		e->get<CTransform>()->dir = dir;
		e->get<CTransform>()->prev_dir = dir;
		e->get<CAnimation>()->active_anim = &e->get<CAnimation>()->anim_set.animations[state][facing];

		if (e->get<CBPatrol>()) {
			e->get<CBPatrol>()->base_pos = pos;
		}

		if (e->get<CSfx>()) {
			if (e->get<CSfx>()->spawn) {
				snd_mgr->playSoundUnique(e->get<CSfx>()->spawn);
			}
		}
	}
}

sf::Vector2f EntityManager::dirOf(size_t facing) {
	switch (facing) {
		case Entity::FACING_E: return {1, 0}; break;
		case Entity::FACING_NE: return {1, -1}; break;
		case Entity::FACING_N: return {0, -1}; break;
		case Entity::FACING_NW: return {-1, -1}; break;
		case Entity::FACING_W: return {-1, 0}; break;
		case Entity::FACING_SW: return {-1, 1}; break;
		case Entity::FACING_S: return {0, 1}; break;
		case Entity::FACING_SE: return {1, 1}; break;
		default: return {0,0};
	}
}