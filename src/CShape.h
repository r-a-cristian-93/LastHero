#ifndef C_SHAPE
#define C_SHAPE

#include <SFML/Graphics.hpp>
#include <memory>

class CShape {
public:
	sf::CircleShape shape;

	CShape();
	CShape(const sf::CircleShape& s);
};

#endif
