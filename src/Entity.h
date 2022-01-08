#ifndef ENTITY
#define ENTITY

#include "Components.h"

class Entity {
public:
	enum {
		TAG_PLAYER = 0,
		TAG_ENEMY = 1,
		TAG_BULLET = 2,
		TAG_CHILD = 3,
		TAG_MISSLE = 4
	};

	const size_t id;
	const size_t tag;
	bool alive;

	Components components;

	Entity(size_t tag, size_t id);

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
