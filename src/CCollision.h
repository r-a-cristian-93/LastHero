#ifndef C_COLLISION
#define C_COLLISION

#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

class Entity;

class CCollision {
public:
	int radius;
	std::map<size_t, sf::Vector2f> offset;
	std::vector<std::shared_ptr<Entity>> colliders;

	CCollision();
	CCollision(const CCollision& c);
	CCollision(int r);
};

#endif
