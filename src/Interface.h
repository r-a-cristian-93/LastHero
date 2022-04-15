#ifndef INTERFACE
#define INTERFACE

#include "Widget.h"

typedef std::vector<Widget> WidgetVec;

class Interface {
	WidgetVec widgets;

public:
	Interface();
	~Interface();

	void add(Widget& widget);
	void update();
	WidgetVec& getWidgets();
};

#endif
