#ifndef C_SHAPE
#define C_SHAPE

#include <SFML/Graphics.hpp>
#include <memory>

class CShape {
public:
	std::shared_ptr<sf::CircleShape> shape;

	CShape();
	CShape(std::shared_ptr<sf::CircleShape>& s);
};

#endif
