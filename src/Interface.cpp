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

void Interface::setLinks(std::string* links[Widget::LINK_COUNT]) {
	setLinks(widgets, links);
}

void Interface::setLinks(WidgetVec& widgets, int* links[Widget::LINK_COUNT]) {
	for (Widget& w : widgets) {
		if (w.get<WCText>() != nullptr) {
			if (w.get<WCText>()->hasLink()) {
				w.get<WCText>()->getLink().setData(&*links[static_cast<unsigned int> (w.get<WCText>()->getLink().getTarget())]);
			}
		}

		if (!w.childs.empty()) setLinks(w.childs, links);
	}
}

void Interface::setLinks(WidgetVec& widgets, std::string* links[Widget::LINK_COUNT]) {
	for (Widget& w : widgets) {
		if (w.get<WCText>() != nullptr) {
			if (w.get<WCText>()->hasLink()) {
				w.get<WCText>()->getLink().setData(&*links[static_cast<unsigned int> (w.get<WCText>()->getLink().getTarget())]);
			}
		}

		if (!w.childs.empty()) setLinks(w.childs, links);
	}
}

Interface::~Interface() {}
