#ifndef WIDGET_BOX
#define	WIDGET_BOX

#include "SFML/Graphics.hpp"
#include "Widget.h"
#include "Border.h"

class WidgetBox: public Widget {
	sf::Sprite* background;
	Border* border;

public:
	void setPosAbs(sf::Vector2i p) override;

	void setBackground(sf::Texture& tex, int offset);
	void setBackground(sf::Sprite& sprite, sf::Vector2i offset);
	void setBorder(Border& b);

	WidgetBox();
	~WidgetBox();
};

#endif
