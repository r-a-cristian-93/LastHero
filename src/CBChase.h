#ifndef C_B_CHASE
#define C_B_CHASE

#include <memory>
#include <vector>
#include "CommonBehaviour.h"

class Entity;

class CBChase {
public:
	// order of conditions determines the priority. First element has the highest priority.
	std::vector<BCondition> cond;
	std::shared_ptr<Entity> target;

	CBChase();
	CBChase(const CBChase& c);
	CBChase(std::vector<BCondition>& cond);
};

#endif
