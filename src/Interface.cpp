#include "Interface.h"
#include <iostream>

Interface::Interface() {
}

void Interface::add(Widget& widget) {
	widgets.push_back(widget);
}

WidgetVec& Interface::getWidgets() {
	return widgets;
}

void Interface::update() {
	for (Widget& w : widgets) {
		w.update();
	}
}

void Interface::setLinks(int* links[Widget::LINK_COUNT]) {
	setLinks(widgets, links);
}

void Interface::setLinks(WidgetVec& widgets, int* links[Widget::LINK_COUNT]) {
	for (Widget& w : widgets) {
		if (w.text && w.link != Widget::LINK_NONE)
			w.linkToInt(*links[w.link]);

		if (!w.childs.empty()) setLinks(w.childs, links);
	}
}

Interface::~Interface() {}
