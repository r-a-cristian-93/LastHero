#ifndef S_DRAW
#define S_DRAW

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.h"
#include "EntityManager.h"
#include "Interface.h"


namespace SDraw {
	void drawEntities(sf::RenderTarget* w, EntityVec& entities);
	void drawInterface(sf::RenderTarget* w, const WidgetVec& widget);
	void drawWidget(sf::RenderTarget* w, Widget& widget);

	bool comparePosition(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
}

#endif
