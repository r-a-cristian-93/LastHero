#ifndef ENTITY
#define ENTITY

#include <memory>
#include <tuple>

#include "CTransform.h"
#include "CCollision.h"
#include "CShape.h"
#include "CBox.h"
#include "CInput.h"
#include "CLifespan.h"
#include "CScore.h"
#include "CTarget.h"
#include "CCooldown.h"

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

	std::shared_ptr<CTransform> c_transform;
	std::shared_ptr<CShape> c_shape;
	std::shared_ptr<CCollision> c_collision;
	std::shared_ptr<CBox> c_box;
	std::shared_ptr<CInput> c_input;
	std::shared_ptr<CLifespan> c_lifespan;
	std::shared_ptr<CScore> c_score;
	std::shared_ptr<CTarget> c_target;
	std::shared_ptr<CCooldown> c_cooldown;

	std::tuple<
		CTransform*,
		CShape*,
		CCollision*,
		CBox*,
		CInput*,
		CLifespan*,
		CScore*,
		CTarget*,
		CCooldown*>
	components;

	Entity(size_t tag, size_t id);

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
