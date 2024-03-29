#ifndef ACTION_MANAGER
#define ACTION_MANAGER

#include <map>
#include <deque>
#include "Action.h"

//map<key, action_code>
typedef std::map<int, Action::Code> ActionMap;
//map <device, ActionMap>
typedef std::map<int, ActionMap> DeviceMap;

class ActionManager {
public:
	enum {
		DEV_KEYBOARD,
		DEV_MOUSE,
		DEV_JOYSTICK
	};

	DeviceMap actions;

	ActionManager();

	void registerAction(int device, int key, Action::Code code);
	Action::Code getCode(int device, int key);
	void reset();
};

#endif
