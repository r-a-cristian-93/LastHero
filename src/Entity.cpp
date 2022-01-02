#include "Entity.h"
	
Entity::Entity(size_t _tag, size_t _id)
	:id(_id)
	,tag(_tag)
	,alive(true)
	,c_transform(nullptr)
	,c_shape(nullptr)
	,c_collision(nullptr)	
	,c_box(nullptr)
	,c_input(nullptr)
	,c_lifespan(nullptr)
	,c_score(nullptr)
	,c_target(nullptr)
	,c_cooldown(nullptr)
	{}

