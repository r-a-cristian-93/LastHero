#ifndef REPLAY_MANAGER
#define REPLAY_MANAGER

#include "ActionStream.h"

class ReplayManager {
public:
	ActionStream stream;
	int frame;
	int cursor;

	Action* nextAction;

	ReplayManager();

	void log(Action*& a);

	void start();
	void pause();
	void stop();
	void rewind(int i);
	void fastForward(int i);

	void next();
	void previous();

	void load();
	void save();
};

#endif
