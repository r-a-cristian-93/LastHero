#ifndef ACTION_MANAGER
#define ACTION_MANAGER

#include <map>
#include "Action.h"


typedef std::map<int, int> ActionMap;
typedef std::map<int, ActionMap> DeviceMap;

class ActionManager {
public:
	enum {
		DEV_KEYBOARD,
		DEV_MOUSE,
		DEV_JOYSTICK
	};

	DeviceMap actions;

	void registerAction(int device, int key, int code);
	int getCode(int device, int key);
};

#endif
