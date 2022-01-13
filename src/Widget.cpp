#include "Widget.h"
#include <iostream>

Widget::Widget()
	:child(nullptr)
	,text(nullptr)
	{}

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
