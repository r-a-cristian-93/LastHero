#include "SDraw.h"
#include <iostream>

void SDraw::drawEntities(sf::RenderWindow* w, const EntityVec& entities) {
	for(const std::shared_ptr<Entity>& e:entities) {
		if (e->get<CShape>()) {
			w->draw(e->get<CShape>()->shape);
		}
	}
}

void SDraw::drawInterface(sf::RenderWindow* w, const WidgetVec& widgets) {
	for(const Widget& widget:widgets) {
		drawWidget(w, widget);
	}
}

void SDraw::drawWidget(sf::RenderWindow* w, const Widget& widget) {
	if (widget.child) {
		drawWidget(w, *(widget.child));
	}

	if (widget.text) {
		w->draw(*(widget.text));
	}
}
