#ifndef ENTITY
#define ENTITY

#include "Components.h"

class Entity {
public:
	enum {
		NONE,
		TAG_PLAYER,
		TAG_ENEMY,
		TAG_PROJECTILE,
		TAG_MISSLE,
		TAG_SFX,
	};

	enum {
		STATE_IDLE = 1,
		STATE_RUN,
		STATE_DIE,
		STATE_SPAWN,
		STATE_HIT,
		STATE_FIRE_PRIMARY,
		STATE_FIRE_SECONDARY
	};


	enum {
		FACING_E = 1,
		FACING_NE,
		FACING_N,
		FACING_NW,
		FACING_W,
		FACING_SW,
		FACING_S,
		FACING_SE
	};

	const size_t id;
	const size_t tag;
	std::string* name;
	bool alive;
	size_t facing;
	size_t state;
	bool blocked;
	bool hit;

	Components components;

	Entity(size_t _tag, size_t _id);
	Entity(size_t _tag, size_t _id, Components& _components);
	Entity(size_t _tag, size_t _id, std::string& name, Components& _components);

	template<class T>
	void add(T* t) {
		components.add<T>(t);
	}

	template<class T>
	T* get() {
		return components.get<T>();
	}
};

#endif
