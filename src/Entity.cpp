#include "Entity.h"

Entity::Entity(size_t _tag, size_t _id)
	:id(_id)
	,tag(_tag)
	,alive(true)
	,facing(FACING_S)
	,state(STATE_IDLE)
	,components()
	{}

Entity::Entity(size_t _tag, size_t _id, Components& _components)
	:id(_id)
	,tag(_tag)
	,alive(true)
	,facing(FACING_S)
	,state(STATE_IDLE)
	,components(_components)
	{}
