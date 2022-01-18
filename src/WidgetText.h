#ifndef WIDGET_TEXT
#define	WIDGET_TEXT

#include "SFML/Graphics.hpp"
#include "Widget.h"
#include "Border.h"

class WidgetText: public Widget {
	std::string str;
	sf::Text* text;

public:
	void setPosAbs(sf::Vector2i p) override;

	void setText(std::string t, sf::Font& font, unsigned int size);
	void setText(std::string t);

	WidgetText();
	~WidgetText();
};

#endif
