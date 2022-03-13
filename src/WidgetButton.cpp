#include "WidgetButton.h"
#include <iostream>

WidgetButton::WidgetButton()
	:state(STATE_IDLE)
	,background_hover(nullptr)
	,border_hover(nullptr)
	{}


void WidgetButton::setPosAbs(sf::Vector2i p) {
	WidgetBox::setPosAbs(p);

	if (background_hover) background_hover->setPosition(pos_abs.x + bg_offset.x, pos_abs.y + bg_offset.y);
	if (border_hover) border_hover->match(sf::IntRect(pos_abs.x, pos_abs.y, size.x, size.y));
}


void WidgetButton::setBackgroundHover(sf::Texture& tex, int offset) {
	if (!background_hover) {
		bg_offset = {offset, offset};
		background_hover = new sf::Sprite(tex, sf::IntRect(0, 0, size.x-offset*2, size.y-offset*2));
		background_hover->setPosition(pos_abs.x+bg_offset.x, pos_abs.y+bg_offset.y);
		drawables_hover.push_back(background_hover);
	}
}

void WidgetButton::setBackgroundHover(sf::Sprite& sprite, sf::Vector2i offset) {
	if (!background_hover) {
		bg_offset = offset;
		background_hover = new sf::Sprite(sprite);
		background_hover->setPosition(pos_abs.x+bg_offset.x, pos_abs.y+bg_offset.y);
		drawables_hover.push_back(background_hover);
	}
}

void WidgetButton::setBorderHover(Border& b) {
	if (!border_hover) {
		border_hover = new Border(b);

		border_hover->match(sf::IntRect(pos_abs.x, pos_abs.y, size.x, size.y));

		for (sf::Sprite* sprite:border_hover->getSprites()) {
			drawables_hover.push_back(sprite);
		}
	}
}

std::vector<sf::Drawable*>& WidgetButton::getDrawables() {
	switch (state) {
		case STATE_HOVER:
			return drawables_hover;
		break;
		case STATE_IDLE:
		default:
			return Widget::getDrawables();
		break;
	}
}
