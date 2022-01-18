#include "WidgetText.h"
#include <iostream>

WidgetText::WidgetText()
	:text(nullptr)
	{}

void WidgetText::setPosAbs(sf::Vector2i p) {
	Widget::setPosAbs(p);

	if (text) text->setPosition(pos_abs.x, pos_abs.y);
}

void WidgetText::setText(std::string t, sf::Font& font, unsigned int size) {
	if (!text) {
		str = t;
		text = new sf::Text(str, font, size);
		drawables.push_back(text);
	}
}

void WidgetText::setText(std::string t) {
	if (text) {
		str = t;
		text->setString(str);
	}
}

WidgetText::~WidgetText() {
	if (text) delete text;
}
