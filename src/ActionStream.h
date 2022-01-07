#ifndef ACTIONS_STREAM
#define ACTIONS_STREAM

#include <deque>
#include "Action.h"




class ActionStream {
public:
	std::deque<Action*> actions;

	ActionStream();
	~ActionStream();
	ActionStream& operator << (Action*& a);
	ActionStream& operator >> (Action*& a);
	
	bool empty();
	void clear();
};

#endif
