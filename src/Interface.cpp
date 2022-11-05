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

void Interface::setLinks(std::variant<int*, std::string*>* links) {
	setLinks(widgets, links);
}

void Interface::setLinks(WidgetVec& widgets, std::variant<int*, std::string*>* links) {
	for (Widget& w : widgets) {
		if (w.get<WCText>() != nullptr) {
			if (w.get<WCText>()->hasLink()) {
				w.get<WCText>()->getLink().m_data = links[static_cast<unsigned int> (w.get<WCText>()->getLink().m_target)];
			}
		}

		if (!w.childs.empty()) setLinks(w.childs, links);
	}
}

Interface::~Interface() {}
