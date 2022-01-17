#include "WidgetBox.h"
#include <iostream>

WidgetBox::WidgetBox()
	:background(nullptr)
	,border(nullptr)
	{}

void WidgetBox::setPosAbs(sf::Vector2i p) {
	Widget::setPosAbs(p);

	if (background) background->setPosition(pos_abs.x, pos_abs.y);
	if (border) border->match(sf::IntRect(pos_abs.x, pos_abs.y, size.x, size.y));
}


void WidgetBox::setBackground(sf::Texture& tex, int offset) {
	if (!background) {
		background = new sf::Sprite(tex, sf::IntRect(0, 0, size.x-offset*2, size.y-offset*2));
		background->setPosition(pos_abs.x+offset, pos_abs.y+offset);
		drawables.push_back(background);
	}
}

void WidgetBox::setBackground(sf::Sprite& sprite, sf::Vector2i offset) {
	if (!background) {
		background = new sf::Sprite(sprite);
		background->setPosition(pos_abs.x+offset.x, pos_abs.y+offset.y);
		drawables.push_back(background);
	}
}

void WidgetBox::setBorder(Border& b) {
	if (!border) {
		border = new Border(b);

		border->match(sf::IntRect(pos_abs.x, pos_abs.y, size.x, size.y));

		for (sf::Sprite* sprite:border->getSprites()) {
			drawables.push_back(sprite);
		}
	}
}

WidgetBox::~WidgetBox() {
	if (border) delete border;
	if (background) delete background;
}
