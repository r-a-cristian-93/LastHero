#ifndef C_B_CHASE
#define C_B_CHASE

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "CommonBehaviour.h"

class Entity;

class CBChase {
public:
	// order of conditions determines the priority. Last element has the highest priority.
	std::vector<BCondition> cond;
	std::shared_ptr<Entity> target;
	std::vector<sf::Vector2f> path;

	CBChase();
	CBChase(const CBChase& c);
	CBChase(std::vector<BCondition>& cond);
};

#endif
