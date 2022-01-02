#ifndef C_BOX
#define C_BOX

#include <SFML/Graphics.hpp>

class CBox {
public:
	sf::FloatRect box;
	
	CBox();
	CBox(sf::FloatRect box);	
};

#endif
