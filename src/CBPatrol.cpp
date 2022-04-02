#include "CBPatrol.h"

CBPatrol::CBPatrol()
	:patrol(0)
	,dist(0)
	,base_pos()
	{}

CBPatrol::CBPatrol(const CBPatrol& c)
	:patrol(c.patrol)
	,dist(c.dist)
	,base_pos(c.base_pos)
	{}

CBPatrol::CBPatrol(size_t _patrol, size_t _dist, sf::Vector2f _base_pos)
	:patrol(_patrol)
	,dist(_dist)
	,base_pos(_base_pos)
	{}
