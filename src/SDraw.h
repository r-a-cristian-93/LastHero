#ifndef S_DRAW
#define S_DRAW

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.h"
#include "EntityManager.h"
#include "Interface.h"


namespace SDraw {
	void drawEntities(sf::RenderTarget* w, const EntityVec& entities);
	void drawInterface(sf::RenderTarget* w, const WidgetVec& widget);
	void drawWidget(sf::RenderTarget* w, Widget& widget);
}

#endif
