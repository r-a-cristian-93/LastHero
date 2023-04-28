#ifndef SYSTEM_DRAW_INTERFACE
#define SYSTEM_DRAW_INTERFACE

#include "System.h"
#include "Interface.h"

class SystemDrawInterface: public System {
private:
	Interface& interface;
	sf::RenderTarget* screen_tex;

public:
	SystemDrawInterface(Interface& _interface, sf::RenderTarget* _screen_tex)
		:interface(_interface)
		,screen_tex(_screen_tex)
	{}

	void operator() () {
		for (Widget& widget : interface.getWidgets()) {
			drawWidget(widget);
		}
	}

private:
	void drawWidget(Widget& widget) {
		for (sf::Drawable* d:widget.getDrawables()) {
			screen_tex->draw(*d);
		}

		screen_tex->draw(widget);

		if (!widget.getChilds().empty()) {
			for (Widget& child:widget.getChilds()) {
				drawWidget(child);
			}
		}
	}
};

#endif