#include "ActionStream.h"
#include <iostream>

ActionStream::ActionStream()
	{}
	
ActionStream::~ActionStream() {
	clear();
}	

void ActionStream::clear() {
	for (size_t i=0; i<actions.size(); i++) {
		delete actions[i];
	}
	
	actions.clear();
}

ActionStream& ActionStream::operator << (Action*& a) {
	actions.push_back(a);
	return *this;
}

ActionStream& ActionStream::operator >> (Action*& a) {
	if (!actions.empty()) {		
		a = actions.front();
		actions.pop_front();
	}
	
	return *this;
}

bool ActionStream::empty() {
	return actions.empty();
}
