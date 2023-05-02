#ifndef SYSTEM_WIDGET_FX_H
#define SYSTEM_WIDGET_FX_H

#include "System.h"
#include "ScenePlayData.h"
#include "Interface.h"

class SystemWidgetFx: public System {
private:
	ScenePlayData& play_data;
	Interface& interface;

public:
	SystemWidgetFx(Interface& _interface, ScenePlayData& _play_data)
		:interface(_interface)
		,play_data(_play_data)
	{}

	void operator() () {
		WidgetVec& widgets = interface.getWidgets();

		for (Widget& w: widgets) {
			handleWidgetFx(w);
		}
	}

	void handleWidgetFx(Widget& w) {
		for (WidgetFx& wfx: w.fx) {
			if (wfx.cond.trigger == TR::PLAYER_HURT && play_data.player->hit) {
				w.current_fx = &wfx;
			}
		}

		if (!w.childs.empty()) {
			for (Widget& child: w.getChilds()) {
				handleWidgetFx(child);
			}
		}
	}
};

#endif