#include "ActionStream.h"
#include <iostream>

ActionStream::ActionStream()
	{}

ActionStream& ActionStream::operator << (Action* a) {
	actions.push_back(a);
	return *this;
}

ActionStream& ActionStream::operator >> (Action* a) {
	if (!actions.empty()) {
		a = actions.front();
		actions.pop_front();
	}

	return *this;
}
