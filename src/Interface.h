#ifndef INTERFACE
#define INTERFACE

#include "Widget.h"
#include "WidgetBox.h"

typedef std::vector<Widget*> WidgetVec;

class Interface {
	WidgetVec widgets;

public:
	Interface();
	~Interface();

	void add(Widget* widget);
	WidgetVec& getWidgets();
};

#endif
