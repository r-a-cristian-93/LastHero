#ifndef SYSTEM_POWERUP_H
#define SYSTEM_POWERUP_H

#include "System.h"
#include "ScenePlayData.h"
#include "CheckCollision.h"

class SystemPowerup: public System {
private:
	ScenePlayData& play_data;

public:
	SystemPowerup(ScenePlayData& _play_data): play_data(_play_data) {}

	void operator() () {
	for (std::shared_ptr<Entity>& e : play_data.ent_mgr.getEntities(TAG::POWERUP)) {
			if (e->get<CBPowerup>()) {
				CBPowerup& cb_powerup = *e->get<CBPowerup>();
				bool cond_met = false;

				switch (cb_powerup.cond.trigger) {
					case TR::PLAYER_NEARBY:
						if (CheckCollision::of(e, play_data.player)) {
							cond_met = true;
						}
					break;
				}

				if (cond_met) {
					switch (cb_powerup.powerup) {
						case CBPowerup::PLAYER_HP:
						{
							const int& initial_hp = play_data.player->get<CStats>()->initial[CStats::HEALTH];
							int& effective_hp = play_data.player->get<CStats>()->effective[CStats::HEALTH];
							int hp_value = cb_powerup.percent * initial_hp / 100;

							effective_hp += hp_value;
							if (effective_hp > initial_hp) effective_hp = initial_hp;
						}
						break;
						case CBPowerup::BASE_HP:
						{
							const int& initial_hp = play_data.base->get<CStats>()->initial[CStats::HEALTH];
							int& effective_hp = play_data.base->get<CStats>()->effective[CStats::HEALTH];
							int hp_value = cb_powerup.percent * initial_hp / 100;

							effective_hp += hp_value;
							if (effective_hp > initial_hp) effective_hp = initial_hp;
						}
						break;
						case CBPowerup::WEAPON_ROUNDS:
							const int& s_rounds = play_data.player->get<CWeapon>()->s_rounds;
							int& s_rounds_current = play_data.player->get<CWeapon>()->s_rounds_current;
							int rounds = cb_powerup.percent * s_rounds / 100;

							s_rounds_current += rounds;
							if (s_rounds_current > s_rounds) s_rounds_current = s_rounds;
						break;
					}

					cb_powerup.percent = 0;
					e->alive = false;
				}
			}
		}
	}
};

#endif