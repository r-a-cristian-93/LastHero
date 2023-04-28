#ifndef SYSTEM_H
#define SYSTEM_H

#include "ScenePlayData.h"

class System {
protected:
	ScenePlayData& play_data;

public:
	System(ScenePlayData& _play_data): play_data(_play_data) {}

	virtual void operator() () = 0;
};

#endif