#include "SDraw.h"
#include <iostream>

void SDraw::drawEntities(sf::RenderWindow* w, const EntityVec& entities) {
	for(const std::shared_ptr<Entity>& e:entities) {
		if (e->c_shape) {
			w->draw(*(e->c_shape->shape));
		}
	}	
}
