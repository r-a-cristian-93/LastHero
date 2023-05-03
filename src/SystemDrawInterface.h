#ifndef SYSTEM_DRAW_INTERFACE
#define SYSTEM_DRAW_INTERFACE

#include "System.h"
#include "Interface.h"
#include "SharedResources.h"

class SystemDrawInterface: public System {
private:
	Interface& interface;

public:
	SystemDrawInterface(Interface& _interface)
		:interface(_interface)
	{}

	void operator() () {
		for (Widget& widget : interface.getWidgets()) {
			drawWidget(widget);
		}
	}

private:
	void drawWidget(Widget& widget) {
		screen_tex->draw(widget);

		if (!widget.getChilds().empty()) {
			for (Widget& child:widget.getChilds()) {
				drawWidget(child);
			}
		}
	}
};

#endif