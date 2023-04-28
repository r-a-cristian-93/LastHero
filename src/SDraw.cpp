#include "SDraw.h"
#include <iostream>

void SDraw::drawInterface(sf::RenderTarget* w, WidgetVec& widgets) {
	for (Widget& widget:widgets) {
		drawWidget(w, widget);
	}
}

void SDraw::drawWidget(sf::RenderTarget* w, Widget& widget) {
	for (sf::Drawable* d:widget.getDrawables()) {
		w->draw(*d);
	}

	w->draw(widget);

	if (!widget.getChilds().empty()) {
		for (Widget& child:widget.getChilds()) {
			drawWidget(w, child);
		}
	}
}
