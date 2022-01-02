#include "CShape.h"

CShape::CShape()
	:shape(nullptr)
	{}
	
CShape::CShape(std::shared_ptr<sf::CircleShape>& s) {
	shape = s;
}
