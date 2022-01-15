#include "Interface.h"
#include <iostream>

Interface::Interface() {
}

Widget* Interface::add() {
	widgets.push_back(new Widget());
	return widgets.back();
}

WidgetVec& Interface::getWidgets() {
	return widgets;
}

Interface::~Interface() {
	for (Widget*& widget:widgets) {
		if (widget) {
			delete widget;
		}
	}

	widgets.clear();
}
