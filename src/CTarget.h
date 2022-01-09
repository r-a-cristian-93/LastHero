#ifndef C_TARGET
#define C_TARGET

#include <memory>
//#include "Entity.h"

class Entity;

class CTarget {
public:
	std::shared_ptr<Entity> target;
	
	CTarget();
	CTarget(const CTarget& c);
	CTarget(std::shared_ptr<Entity> t);
};

#endif
