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
	for(Widget* widget:widgets) {
		drawWidget(w, *widget);
	}
}

void SDraw::drawWidget(sf::RenderWindow* w, const Widget& widget) {
	if (widget.child) {
		drawWidget(w, *(widget.child));
	}

	if (widget.background) {
		w->draw(*(widget.background));
	}

	if (widget.border) {
		w->draw(widget.border->getSprite(Border::TOP_LEFT));
		w->draw(widget.border->getSprite(Border::TOP_CENTER));
		w->draw(widget.border->getSprite(Border::TOP_RIGHT));
	}

	if (widget.text) {
		w->draw(*(widget.text));
	}
}
