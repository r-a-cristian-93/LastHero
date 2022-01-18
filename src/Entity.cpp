#include "Entity.h"

Entity::Entity(size_t _tag, size_t _id)
	:id(_id)
	,tag(_tag)
	,alive(true)
	,components(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr)
	{}

Entity::Entity(size_t _tag, size_t _id, Components& _components)
	:id(_id)
	,tag(_tag)
	,alive(true)
	,components(_components)
	{}
