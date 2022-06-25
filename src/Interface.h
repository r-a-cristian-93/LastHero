#ifndef INTERFACE
#define INTERFACE

#include "Widget.h"

typedef std::vector<Widget> WidgetVec;

class Interface {
	WidgetVec widgets;

	void setLinks(WidgetVec& widgets, int* links[Widget::LINK_COUNT]);
	void setLinks(WidgetVec& widgets, std::string* links[Widget::LINK_COUNT]);

public:
	Interface();
	~Interface();

	void add(Widget& widget);
	void update();
	void setLinks(int* links[Widget::LINK_COUNT]);
	void setLinks(std::string* links[Widget::LINK_COUNT]);
	WidgetVec& getWidgets();
};

#endif
