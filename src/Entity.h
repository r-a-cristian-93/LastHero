#ifndef ENTITY
#define ENTITY

#include "Components.h"

class Entity {
public:
	enum {
		NONE,
		TAG_PLAYER,
		TAG_ENEMY,
		TAG_BULLET,
		TAG_MISSLE,
		TAG_CHILD,
	};

	enum {
		STATE_STAND = 1,
		STATE_MOVE,
		STATE_DIE
	};


	enum {
		FACING_N = 1,
		FACING_S,
		FACING_E,
		FACING_W,
		FACING_NE,
		FACING_NW,
		FACING_SE,
		FACING_SW
	};

	const size_t id;
	const size_t tag;
	bool alive;
	size_t facing;
	size_t state;

	Components components;

	Entity(size_t _tag, size_t _id);
	Entity(size_t _tag, size_t _id, Components& _components);

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
