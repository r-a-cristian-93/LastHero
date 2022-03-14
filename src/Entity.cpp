#include "Entity.h"

Entity::Entity(size_t _tag, size_t _id)
	:id(_id)
	,tag(_tag)
	,name(0)
	,alive(true)
	,facing(FACING_S)
	,state(STATE_IDLE)
	,blocked(false)
	,hit(false)
	,components()
	{}

Entity::Entity(size_t _tag, size_t _id, Components& _components)
	:id(_id)
	,tag(_tag)
	,name(0)
	,alive(true)
	,facing(FACING_S)
	,state(STATE_IDLE)
	,blocked(false)
	,hit(false)
	,components(_components)
	{}

Entity::Entity(size_t _tag, size_t _id, size_t _name, Components& _components)
	:id(_id)
	,tag(_tag)
	,name(_name)
	,alive(true)
	,facing(FACING_S)
	,state(STATE_IDLE)
	,blocked(false)
	,hit(false)
	,components(_components)
	{}
