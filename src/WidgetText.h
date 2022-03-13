#ifndef WIDGET_TEXT
#define	WIDGET_TEXT

#include "SFML/Graphics.hpp"
#include "Widget.h"
#include "Border.h"

class WidgetText: public Widget {
	size_t link_text;
	int* link_int;
	sf::Text* text;

public:
	enum {
		LINK_NONE,
		LINK_PLAYER_HP,
		LINK_BASE_HP,
		LINK_TOTAL_KILLS,
	};

	void setPosAbs(sf::Vector2i p) override;

	void setText(std::string t, sf::Font& font, unsigned int size);
	void setText(std::string t);
	void setColor(sf::Color color);
	void linkToInt(int& value);
	void updateText();
	void updateOrigin();

	WidgetText();
	~WidgetText();
};

#endif
