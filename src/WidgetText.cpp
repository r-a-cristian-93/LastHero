#include "WidgetText.h"
#include <iostream>

WidgetText::WidgetText()
	:text(nullptr)
	,link_text(LINK_NONE)
	,link_int(nullptr)
	{}

void WidgetText::setPosAbs(sf::Vector2i p) {
	Widget::setPosAbs(p);

	if (text) text->setPosition(pos_abs.x, pos_abs.y);
}

void WidgetText::setText(std::string t, sf::Font& font, unsigned int size) {
	if (!text) {
		text = new sf::Text(t, font, size);
		drawables.push_back(text);
		updateOrigin();
	}
}

void WidgetText::setText(std::string t) {
	if (text) {
		text->setString(t);
		updateOrigin();
	}
}

void WidgetText::linkToInt(int& value) {
	link_int = & value;
}

void WidgetText::updateText() {
	if (link_int)  {
		setText(std::to_string(*link_int));
		updateOrigin();
	}
}

void WidgetText::updateOrigin() {
	sf::FloatRect b = text->getLocalBounds();
	text->setOrigin(b.left + b.width/2, b.top + b.height/2);
}


WidgetText::~WidgetText() {
	if (text) delete text;
}
