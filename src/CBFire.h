#ifndef C_B_FIRE
#define C_B_FIRE

#include <memory>
#include <vector>
#include "CommonBehaviour.h"

class Entity;

class CBFire {
public:
	std::vector<BCondition> pri;
	std::vector<BCondition> sec;
	std::shared_ptr<Entity> target;

	CBFire();
	CBFire(const CBFire& c);
	CBFire(std::vector<BCondition> pri, std::vector<BCondition> sec);
};

#endif
