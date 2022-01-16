#include "WidgetBox.h"
#include <iostream>

WidgetBox::WidgetBox()
	:background(nullptr)
	,border(nullptr)
	{}

void WidgetBox::setPosAbs(sf::Vector2i p) {
	Widget::setPosAbs(p);

	if (background) background->setPosition(pos_abs.x, pos_abs.y);
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

		sf::Sprite& top_left = border->getSprite(Border::TOP_LEFT);
		sf::Sprite& top_center = border->getSprite(Border::TOP_CENTER);
		sf::Sprite& top_right = border->getSprite(Border::TOP_RIGHT);
		sf::Sprite& bottom_left = border->getSprite(Border::BOTTOM_LEFT);
		sf::Sprite& bottom_center = border->getSprite(Border::BOTTOM_CENTER);
		sf::Sprite& bottom_right = border->getSprite(Border::BOTTOM_RIGHT);
		sf::Sprite& middle_left = border->getSprite(Border::MIDDLE_LEFT);
		sf::Sprite& middle_right = border->getSprite(Border::MIDDLE_RIGHT);

		top_left.setPosition(pos_abs.x, pos_abs.y);
		top_center.setPosition(pos_abs.x + top_left.getTextureRect().width, pos_abs.y);
		top_right.setPosition(pos_abs.x + size.x - top_right.getTextureRect().width, pos_abs.y);
		bottom_left.setPosition(pos_abs.x, pos_abs.y + size.y - bottom_left.getTextureRect().height);
		bottom_center.setPosition(pos_abs.x + bottom_left.getTextureRect().width, pos_abs.y + size.y - bottom_center.getTextureRect().height);
		bottom_right.setPosition(pos_abs.x + size.x - bottom_right.getTextureRect().width, pos_abs.y + size.y - bottom_right.getTextureRect().height);
		middle_left.setPosition(pos_abs.x, pos_abs.y + top_left.getTextureRect().height);
		middle_right.setPosition(pos_abs.x + size.x - middle_right.getTextureRect().width, pos_abs.y + top_right.getTextureRect().height);

		top_center.setTextureRect(sf::IntRect(0,0, size.x - top_left.getTextureRect().width - top_right.getTextureRect().width, top_center.getTextureRect().height));
		bottom_center.setTextureRect(sf::IntRect(0,0, size.x - bottom_left.getTextureRect().width - bottom_right.getTextureRect().width, bottom_center.getTextureRect().height));
		middle_left.setTextureRect(sf::IntRect(0,0, middle_left.getTextureRect().width, size.y - top_left.getTextureRect().height - bottom_left.getTextureRect().height));
		middle_right.setTextureRect(sf::IntRect(0,0, middle_right.getTextureRect().width, size.y - top_right.getTextureRect().height - bottom_right.getTextureRect().height));
	}
}

WidgetBox::~WidgetBox() {
	if (border) delete border;
	if (background) delete background;
}
