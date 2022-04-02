#ifndef C_B_PATROL
#define C_B_PATROL

#include <SFML/Graphics.hpp>

class CBPatrol {
public:
	enum {
		NONE = 0,
		PATROL_HORIZONTAL,
		PATROL_VERTICAL,
		PATROL_SQUARE
	};

	size_t patrol;
	size_t dist;
	sf::Vector2f base_pos;

	CBPatrol();
	CBPatrol(const CBPatrol& c);
	CBPatrol(size_t patrol, size_t dist, sf::Vector2f base_pos);
};

#endif
