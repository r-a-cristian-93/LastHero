#ifndef S_DRAW
#define S_DRAW

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.h"
#include "EntityManager.h"
#include "Interface.h"


namespace SDraw {
	void drawEntities(sf::RenderWindow* w, const EntityVec& entities);
	void drawInterface(sf::RenderWindow* w, const WidgetVec& widget);
	void drawWidget(sf::RenderWindow* w, const Widget& widget);
}

#endif
