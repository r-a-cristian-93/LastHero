#include "Widget.h"
#include <iostream>

Widget::Widget()
	:pos_rel(0, 0)
	,pos_abs(0, 0)
	,size(0, 0)
	,background(nullptr)
	,bg_offset(0,0)
	,border(nullptr)
	,text(nullptr)
	,link_int(nullptr)
	{}

Widget::Widget(const Widget& w)
	:pos_rel(w.pos_rel)
	,pos_abs(w.pos_abs)
	,size(w.size)
	,background(nullptr)
	,border(nullptr)
	,text(nullptr)
	,link_int(w.link_int)
	,childs(w.childs)
{
	if (w.background) setBackground(*w.background, w.bg_offset);
	if (w.border) setBorder(*w.border);
	if (w.text) setText(*w.text);
}


Widget::~Widget() {
	delete background;
	delete border;
	delete text;
}

void Widget::setPosRel(sf::Vector2i p) {
	pos_rel = p;

	if (!childs.empty()) {
		for (int i=0; i<childs.size(); i++) {
			updateChildPos(childs[i]);
		}
	}
}

void Widget::setPosAbs(sf::Vector2i p) {
	pos_abs = p + pos_rel;

	if (!childs.empty()) {
		for (int i=0; i<childs.size(); i++) {
			updateChildPos(childs[i]);
		}
	}

	if (background) background->setPosition(pos_abs.x + bg_offset.x, pos_abs.y + bg_offset.y);
	if (border) border->match(sf::IntRect(pos_abs.x, pos_abs.y, size.x, size.y));
	if (text) text->setPosition(pos_abs.x, pos_abs.y);
}

void Widget::setSize(sf::Vector2i s) {
	size = s;
}

void Widget::addChild(Widget& child) {
	updateChildPos(child);
	childs.push_back(child);
}

void Widget::updateChildPos(Widget& child) {
	child.setPosAbs(pos_abs);
}

std::vector<sf::Drawable*>& Widget::getDrawables() {
	return drawables;
}

std::vector<Widget>& Widget::getChilds() {
	return childs;
}

// BOX

void Widget::setBackground(sf::Texture& tex, int offset) {
	if (!background) {
		bg_offset = {offset, offset};
		background = new sf::Sprite(tex, sf::IntRect(0, 0, size.x-offset*2, size.y-offset*2));
		background->setPosition(pos_abs.x+bg_offset.x, pos_abs.y+bg_offset.y);
		drawables.push_back(background);
	}
}

void Widget::setBackground(sf::Sprite& sprite, sf::Vector2i offset) {
	if (!background) {
		bg_offset = offset;
		background = new sf::Sprite(sprite);
		background->setPosition(pos_abs.x+bg_offset.x, pos_abs.y+bg_offset.y);
		drawables.push_back(background);
	}
}

void Widget::setBackgroundColor(sf::Color color) {
	if (background) background->setColor(color);
}

void Widget::setBorder(Border& b) {
	if (!border) {
		border = new Border(b);

		border->match(sf::IntRect(pos_abs.x, pos_abs.y, size.x, size.y));

		for (sf::Sprite* sprite:border->getSprites()) {
			drawables.push_back(sprite);
		}
	}
}





// TEXT

void Widget::setText(std::string t, sf::Font& font, unsigned int size) {
	if (!text) {
		text = new sf::Text(t, font, size);
		drawables.push_back(text);
		updateOrigin();
	}
}

void Widget::setText(std::string t) {
	if (text) {
		text->setString(t);
		updateOrigin();
	}
}

void Widget::setText(sf::Text& t) {
	if (text) {
		text = new sf::Text(t);
		drawables.push_back(text);
		updateOrigin();
	}
}

void Widget::linkToInt(int& value) {
	link_int = & value;
}

void Widget::updateText() {
	if (link_int)  {
		setText(std::to_string(*link_int));
		updateOrigin();
	}
}

void Widget::updateOrigin() {
	sf::FloatRect b = text->getLocalBounds();
	text->setOrigin(b.left + b.width/2, b.top + b.height/2);
}

void Widget::setTextColor(sf::Color color) {
	if (text) text->setFillColor(color);
}





// BOX and TEXT

void Widget::update() {
	if (text) {
		updateText();
		updateOrigin();
	}

	if (!childs.empty()) {
		for (int i=0; i<childs.size(); i++) {
			childs[i].update();
		}
	}
}

void Widget::setColor(sf::Color color) {
	if (text) text->setFillColor(color);
	if (background) background->setColor(color);
}
