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

void Widget::setBackground(sf::Color color, int offset) {
	if (!background) {
		background = new sf::RectangleShape(sf::Vector2f(size.x-offset*2, size.y-offset*2));
		background->setPosition(pos.x+offset, pos.y+offset);
	}

	background->setFillColor(color);
}

void Widget::setBorder(Border& b) {
	if (!border) {
		border = new Border(b);

		sf::Sprite& top_left = border->getSprite(Border::TOP_LEFT);
		sf::Sprite& top_center = border->getSprite(Border::TOP_CENTER);
		sf::Sprite& top_right = border->getSprite(Border::TOP_RIGHT);

		top_left.setPosition(pos.x, pos.y);
		top_center.setPosition(pos.x + top_left.getTextureRect().width, pos.y);
		top_right.setPosition(pos.x + size.x - top_right.getTextureRect().width, pos.y);
	}
}

Widget::~Widget() {
	if (text) delete text;
	if (background) delete background;
	if (border) delete border;
	if (child) delete child;
}
