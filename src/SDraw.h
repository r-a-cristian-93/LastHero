#ifndef S_DRAW
#define S_DRAW

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.h"
#include "EntityManager.h"
#include "Interface.h"


namespace SDraw {
	void drawInterface(sf::RenderTarget* w, WidgetVec& widget);
	void drawWidget(sf::RenderTarget* w, Widget& widget);
}

#endif
