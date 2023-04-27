#ifndef INTERFACE
#define INTERFACE

#include <variant>
#include "Widget.h"

typedef std::vector<Widget> WidgetVec;

class Interface {
	WidgetVec widgets;

	void setLinks(WidgetVec& widgets, std::variant<std::monostate, int*, std::string*>* links);

public:
	Interface();
	~Interface();

	void add(Widget& widget);
	void update();
	void setLinks(std::variant<std::monostate, int*, std::string*>* links);
	WidgetVec& getWidgets();
};

#endif
