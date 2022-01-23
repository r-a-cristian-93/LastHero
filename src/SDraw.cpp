#include "SDraw.h"
#include <iostream>

void SDraw::drawEntities(sf::RenderWindow* w, const EntityVec& entities) {
	for(const std::shared_ptr<Entity>& e:entities) {
		if (e->get<CShape>()) {
			w->draw(e->get<CShape>()->shape);
		}
		if (e->get<CAnimation>()) {
			w->draw(e->get<CAnimation>()->active_anim->getSprite());
		}
	}
}

void SDraw::drawInterface(sf::RenderWindow* w, const WidgetVec& widgets) {
	for(Widget* widget:widgets) {
		drawWidget(w, *widget);
	}
}

void SDraw::drawWidget(sf::RenderWindow* w, Widget& widget) {
	for (sf::Drawable* d:widget.getDrawables()) {
		w->draw(*d);
	}

	if (!widget.getChilds().empty()) {
		for (Widget* child:widget.getChilds()) {
			drawWidget(w, *(child));
		}
	}
}
