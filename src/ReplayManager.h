#ifndef REPLAY_MANAGER
#define REPLAY_MANAGER

#include "ActionStream.h"

class ReplayManager {
public:
	ActionStream stream;	
	int frame;

	ReplayManager();
	
	void log(Action*& a);
	
	void play();
	void pause();
	void stop();	
	void rewind(int i);
	void fastForward(int i);
	
	void load();
	void save();
};

#endif
