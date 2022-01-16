#include "Interface.h"
#include <iostream>

Interface::Interface() {
}

void Interface::add(Widget* widget) {
	widgets.push_back(widget);
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
