#ifndef INTERFACE
#define INTERFACE

#include <variant>
#include "Widget.h"

typedef std::vector<Widget> WidgetVec;

class Interface {
	WidgetVec widgets;

	void setLinks(WidgetVec& widgets, std::variant<int*, std::string*>* links);

public:
	Interface();
	~Interface();

	void add(Widget& widget);
	void update();
	void setLinks(std::variant<int*, std::string*>* links);
	WidgetVec& getWidgets();
};

#endif
