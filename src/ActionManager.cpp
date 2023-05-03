#include <fstream>

#include "ActionManager.h"

ActionManager::ActionManager()
	{}

void ActionManager::registerAction(int device, int key, Action::Code code) {
	actions[device][key] = code;
}

Action::Code ActionManager::getCode(int device, int key) {
	return actions[device][key];
}

void ActionManager::reset() {
	actions.clear();
}
