#ifndef C_COLLISION
#define C_COLLISION

#include <map>
#include <SFML/Graphics.hpp>

class CCollision {
public:
	int radius;
	std::map<size_t, sf::Vector2f> offset;

	CCollision();
	CCollision(const CCollision& c);
	CCollision(int r);
};

#endif
