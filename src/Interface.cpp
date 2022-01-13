#include "Interface.h"

Interface::Interface() {
}

Widget& Interface::add() {
	widgets.push_back(Widget());
	return widgets.back();
}

WidgetVec& Interface::getWidgets() {
	return widgets;
}
