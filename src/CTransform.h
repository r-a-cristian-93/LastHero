#ifndef C_TRANSFORM
#define C_TRANSFORM

#include <SFML/Graphics.hpp>


class CTransform {
public:
	int max_velocity;

	sf::Vector2f pos;
	sf::Vector2f dir;
	sf::Vector2f vel;
	int d_angle;
	
	CTransform();
	CTransform(const sf::Vector2f& p, const float& v);		
};

#endif
