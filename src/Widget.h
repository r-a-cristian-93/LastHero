#ifndef WIDGET
#define	WIDGET

#include "SFML/Graphics.hpp"
#include "Border.h"

class Widget {
public:
	Widget* child;

	sf::Vector2i pos;
	sf::Vector2i size;

	std::string str;
	sf::Text* text;
	sf::Sprite* background;
	Border* border;

	void setText(std::string t, sf::Font& font, unsigned int size);
	void setText(std::string t);
	void setBackground(sf::Texture& tex, int offset);
	void setBackground(sf::Sprite& sprite, sf::Vector2i offset);
	void setBorder(Border& b);
	void update();

	void setPosition(int x, int y);
	void setSize(int x, int y);

	Widget();
	~Widget();
};

#endif
