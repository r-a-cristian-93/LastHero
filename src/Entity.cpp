#include "Entity.h"
#include <iostream>

Entity::Entity(size_t _tag, size_t _id)
	:id(_id)
	,tag(_tag)
	,alive(true)
	,c_shape(nullptr)
	,c_input(nullptr)
	,c_target(nullptr)
	,c_cooldown(nullptr)
	,components(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr)
	{}

Entity::~Entity() {
	if (get<CTransform>()) delete get<CTransform>();
	if (get<CShape>()) delete get<CShape>();
	if (get<CCollision>()) delete get<CCollision>();
	if (get<CInput>()) delete get<CInput>();
	if (get<CLifespan>()) delete get<CLifespan>();
	if (get<CScore>()) delete get<CScore>();
	if (get<CTarget>()) delete get<CTarget>();
	if (get<CCooldown>()) delete get<CCooldown>();
}
