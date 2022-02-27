#include "SDraw.h"
#include <iostream>
#include <algorithm>

void SDraw::drawEntities(sf::RenderTarget* w, EntityVec& entities) {
	std::sort(entities.begin(), entities.end(), comparePosition);

	for (const std::shared_ptr<Entity>& e:entities) {
#ifdef DRAW_COLLISION_BOX
		if (e->get<CShape>()) {
			//w->draw(e->get<CShape>()->shape);
		}
#endif

		if (e->get<CAnimation>()) {
			w->draw(e->get<CAnimation>()->active_anim->getSprite());
		}
	}
}

void SDraw::drawInterface(sf::RenderTarget* w, const WidgetVec& widgets) {
	for (Widget* widget:widgets) {
		drawWidget(w, *widget);
	}
}

void SDraw::drawWidget(sf::RenderTarget* w, Widget& widget) {
	for (sf::Drawable* d:widget.getDrawables()) {
		w->draw(*d);
	}

	if (!widget.getChilds().empty()) {
		for (Widget* child:widget.getChilds()) {
			drawWidget(w, *(child));
		}
	}
}


bool SDraw::comparePosition(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
	if (a->get<CAnimation>() && b->get<CAnimation>()) {
		if (a->get<CAnimation>()->prio != b->get<CAnimation>()->prio) {
			return (a->get<CAnimation>()->prio < b->get<CAnimation>()->prio);
		}
	}

	if (a->get<CTransform>() && b->get<CTransform>()) {
		return (a->get<CTransform>()->pos.y < b->get<CTransform>()->pos.y);
	}

	return false;
}
