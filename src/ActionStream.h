#ifndef ACTIONS_STREAM
#define ACTIONS_STREAM

#include <deque>
#include "Action.h"




class ActionStream {
public:
	std::deque<Action*> actions;

	ActionStream();
	ActionStream& operator << (Action* a);
	ActionStream& operator >> (Action* a);
};

#endif
