#ifndef C_TRANSFORM
#define C_TRANSFORM

#include <SFML/Graphics.hpp>


class CTransform {
public:
	float max_velocity;

	sf::Vector2f pos;
	sf::Vector2f dir;
	sf::Vector2f prev_pos;
	sf::Vector2f prev_dir;
	sf::Vector2f vel;
	int d_angle;

	CTransform();
	CTransform(const CTransform& c);
	CTransform(const float& v);
	CTransform(const sf::Vector2f& p, const float& v);
	CTransform(const sf::Vector2f& p, const sf::Vector2f& d, const float& v);
};

#endif
