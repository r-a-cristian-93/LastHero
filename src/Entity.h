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

	const size_t id;
	const size_t tag;
	bool alive;

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
