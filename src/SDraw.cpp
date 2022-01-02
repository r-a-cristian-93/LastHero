#include "SDraw.h"
#include <iostream>

void SDraw::drawEntities(sf::RenderWindow* w, const EntityVec& entities) {
	for(const std::shared_ptr<Entity>& e:entities) {
		if (e->get<CShape>()) {
			w->draw(*(e->get<CShape>()->shape));
		}
	}	
}
