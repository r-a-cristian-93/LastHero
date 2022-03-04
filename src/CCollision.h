#ifndef C_COLLISION
#define C_COLLISION

#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

class Entity;

typedef std::map<size_t, sf::Vector2f> DirOffset;

struct HitBox {
	int radius;
	std::map<size_t, sf::Vector2f> offset;
};

class CCollision {
public:
	std::vector<HitBox> hitbox;
	std::vector<std::shared_ptr<Entity>> colliders;

	CCollision();
	CCollision(const CCollision& c);
};

#endif
