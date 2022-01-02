#ifndef ENTITY
#define ENTITY

#include <memory>
#include <tuple>

#include "CTransform.h"
#include "CCollision.h"
#include "CShape.h"
#include "CInput.h"
#include "CLifespan.h"
#include "CScore.h"
#include "CTarget.h"

typedef std::tuple<
	CTransform*,
	CShape*,
	CCollision*,
	CInput*,
	CLifespan*,
	CScore*,
	CTarget*>
Components;

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
	~Entity();

	template<class T>
	void add(T* t) {
		std::get<T*>(components) = t;
	}

	template<class T>
	T* get() {
		return std::get<T*>(components);
	}
};

#endif
