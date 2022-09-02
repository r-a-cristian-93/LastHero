#include "GameStats.h"
#include "SharedResources.h"

void GameStats::addKills(KillsMap kills) {
	new_kills_per_enemy = kills;

	KillsMap::const_iterator it;
	for (it = kills.cbegin(); it!=kills.cend(); it++) {
		kills_per_enemy[it->first] += it->second;
	}
}

bool GameStats::stageNext() {
	prev_stage = next_stage;
	if (next_stage < assets->getStages().size()-1) {
		next_stage++;

		return true;
	}
	return false;
}

size_t GameStats::stagesCount() {
	return assets->getStages().size();
}
