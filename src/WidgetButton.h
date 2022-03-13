#ifndef WIDGET_TEXT_BOX
#define	WIDGET_TEXT_BOX

#include "SFML/Graphics.hpp"
#include "WidgetBox.h"

class WidgetButton: public WidgetBox {
	sf::Sprite* background_hover;
	Border* border_hover;
	std::vector<sf::Drawable*> drawables_hover;

	size_t state;

public:
	enum {
		STATE_NONE = 0,
		STATE_IDLE,
		STATE_HOVER
	};

	void setPosAbs(sf::Vector2i p) override;
	std::vector<sf::Drawable*>& getDrawables() override;

	void setBackgroundHover(sf::Texture& tex, int offset);
	void setBackgroundHover(sf::Sprite& sprite, sf::Vector2i offset);
	void setBorderHover(Border& b);

	WidgetButton();
	~WidgetButton();
};

#endif
