#ifndef GAME_STATS
#define GAME_STATS

#include "SharedResources.h"

class GameStats {
public:
	enum State {
		PLAY,
		WIN,
		LOSE,
		OVER
	};

	typedef std::map<size_t, size_t> KillsMap;

	size_t next_stage;
	size_t prev_stage;

	KillsMap kills_per_enemy;
	KillsMap new_kills_per_enemy;

	size_t state;

	void addKills(KillsMap kills);
	bool stageNext();
	size_t stagesCount();
};

#endif
