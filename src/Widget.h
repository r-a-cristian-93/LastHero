#ifndef WIDGET
#define	WIDGET

#include "SFML/Graphics.hpp"

class Widget {	

public:
	Widget* child;
	std::string str;
	sf::Text* text;

	void setText(std::string t, sf::Font& font, unsigned int size);
	void setText(std::string t);	
	void update();

	Widget();
};

#endif
