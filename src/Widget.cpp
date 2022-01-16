#include "Widget.h"
#include <iostream>

Widget::Widget()
	:child(nullptr)
	,pos(0, 0)
	,size(0, 0)
	,text(nullptr)
	,background(nullptr)
	,border(nullptr)
	{}


void Widget::setPosition(int x, int y) {
	pos.x = x;
	pos.y = y;
}

void Widget::setSize(int x, int y) {
	size.x = x;
	size.y = y;
}

void Widget::setText(std::string t, sf::Font& font, unsigned int size) {
	str = t;
	text = new sf::Text(str, font, size);
}

void Widget::setText(std::string t) {
	if (text) {
		str = t;
		text->setString(str);
	}
}

void Widget::setBackground(sf::Texture& tex, int offset) {
	if (!background) {
		background = new sf::Sprite(tex, sf::IntRect(0, 0, size.x-offset*2, size.y-offset*2));
		background->setPosition(pos.x+offset, pos.y+offset);
	}
}

void Widget::setBackground(sf::Sprite& sprite, sf::Vector2i offset) {
	if (!background) {
		background = new sf::Sprite(sprite);
		background->setPosition(pos.x+offset.x, pos.y+offset.y);
	}
}

void Widget::setBorder(Border& b) {
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

		top_left.setPosition(pos.x, pos.y);
		top_center.setPosition(pos.x + top_left.getTextureRect().width, pos.y);
		top_right.setPosition(pos.x + size.x - top_right.getTextureRect().width, pos.y);
		bottom_left.setPosition(pos.x, pos.y + size.y - bottom_left.getTextureRect().height);
		bottom_center.setPosition(pos.x + bottom_left.getTextureRect().width, pos.y + size.y - bottom_center.getTextureRect().height);
		bottom_right.setPosition(pos.x + size.x - bottom_right.getTextureRect().width, pos.y + size.y - bottom_right.getTextureRect().height);
		middle_left.setPosition(pos.x, pos.y + top_left.getTextureRect().height);
		middle_right.setPosition(pos.x + size.x - middle_right.getTextureRect().width, pos.y + top_right.getTextureRect().height);

		top_center.setTextureRect(sf::IntRect(0,0, size.x - top_left.getTextureRect().width - top_right.getTextureRect().width, top_center.getTextureRect().height));
		bottom_center.setTextureRect(sf::IntRect(0,0, size.x - bottom_left.getTextureRect().width - bottom_right.getTextureRect().width, bottom_center.getTextureRect().height));
		middle_left.setTextureRect(sf::IntRect(0,0, middle_left.getTextureRect().width, size.y - top_left.getTextureRect().height - bottom_left.getTextureRect().height));
		middle_right.setTextureRect(sf::IntRect(0,0, middle_right.getTextureRect().width, size.y - top_right.getTextureRect().height - bottom_right.getTextureRect().height));
	}
}

Widget::~Widget() {
	if (text) delete text;
	if (background) delete background;
	if (border) delete border;
	if (child) delete child;
}
