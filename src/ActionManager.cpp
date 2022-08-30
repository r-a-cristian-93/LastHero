#include <fstream>

#include "ActionManager.h"

ActionManager::ActionManager()
	{}

void ActionManager::registerAction(int device, int key, int code) {
	actions[device][key] = code;
}

int ActionManager::getCode(int device, int key) {
	return actions[device][key];
}

void ActionManager::reset() {
	actions.clear();
}
