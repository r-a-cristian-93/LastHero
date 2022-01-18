#include "Widget.h"
#include <iostream>

Widget::Widget()
	:pos_rel(0, 0)
	,pos_abs(0, 0)
	,size(0, 0)
	{}


void Widget::setPosRel(sf::Vector2i p) {
	pos_rel = p;

	if (!childs.empty()) {
		for (int i=0; i<childs.size(); i++) {
			updateChildPos(*childs[i]);
		}
	}
}

void Widget::setPosAbs(sf::Vector2i p) {
	pos_abs = p + pos_rel;

	if (!childs.empty()) {
		for (int i=0; i<childs.size(); i++) {
			updateChildPos(*childs[i]);
		}
	}
}

void Widget::setSize(sf::Vector2i s) {
	size = s;
}

void Widget::addChild(Widget* child) {
	childs.push_back(child);
	updateChildPos(*childs.back());
}

void Widget::updateChildPos(Widget& child) {
	child.setPosAbs(pos_abs);
}

std::vector<sf::Drawable*>& Widget::getDrawables() {
	return drawables;
}

std::vector<Widget*>& Widget::getChilds() {
	return childs;
}

Widget::~Widget() {
	while (!childs.empty()) {
		delete childs.back();
		childs.pop_back();
	}
}
