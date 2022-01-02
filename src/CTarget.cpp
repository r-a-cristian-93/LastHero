#include "CTarget.h"

CTarget::CTarget()
	:target(nullptr)
	{}
	
CTarget::CTarget(std::shared_ptr<Entity> t)
	:target(t)
	{}
