#ifndef INTERFACE
#define INTERFACE

#include "Widget.h"

typedef std::vector<Widget> WidgetVec;

class Interface {
	WidgetVec widgets;

public:
	Interface();

	Widget& add();
	WidgetVec& getWidgets();
};

#endif
