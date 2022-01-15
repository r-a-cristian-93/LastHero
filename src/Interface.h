#ifndef INTERFACE
#define INTERFACE

#include "Widget.h"

typedef std::vector<Widget*> WidgetVec;

class Interface {
	WidgetVec widgets;

	void deleteWidget(Widget*& widget);

public:
	Interface();
	~Interface();

	Widget* add();
	WidgetVec& getWidgets();
};

#endif
