#include "CTarget.h"

CTarget::CTarget()
	:target(nullptr)
	{}

CTarget::CTarget(const CTarget& c)
	:target(c.target)
	{}
	
CTarget::CTarget(std::shared_ptr<Entity> t)
	:target(t)
	{}
