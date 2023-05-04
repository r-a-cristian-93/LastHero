#ifndef SYSTEM_FIRE_WEAPON_H
#define SYSTEM_FIRE_WEAPON_H

#include "System.h"
#include "ScenePlayData.h"

class SystemFireWeapon: public System {
private:
	ScenePlayData& play_data;

public:
	SystemFireWeapon(ScenePlayData& _play_data): play_data(_play_data)
	{}

	void operator() () {
		for (std::shared_ptr<Entity>& e : play_data.ent_mgr.getEntities()) {
			if (e->get<CWeapon>() && e->alive) {
				CWeapon& comp_w = *e->get<CWeapon>();
				size_t facing(e->facing);

				sf::Vector2f pos(e->get<CTransform>()->pos + e->get<CWeapon>()->projectile_spawn[facing]);

				if (comp_w.p_cooldown_current > 0) {
					comp_w.p_cooldown_current -= app_conf->frame_dt * app_conf->game_speed;
					e->get<CInput>()->fire_primary = false;
				}

				if (comp_w.s_cooldown_current > 0) {
					comp_w.s_cooldown_current -= app_conf->frame_dt * app_conf->game_speed;
					e->get<CInput>()->fire_secondary = false;
				}

				// use only one weapon at a time
				// the weapon cooldown time should be slightly higher than the firing animation
				if (comp_w.p_cooldown_current <= 0 && comp_w.s_cooldown_current <= 0) {
					if (e->get<CInput>()->fire_primary && comp_w.p_rounds_current) {
						if (comp_w.p_delay_current <= 0) {
							play_data.ent_mgr.spawnEntity(comp_w.p_tag, comp_w.primary, e, pos, Entity::STATE_RUN, facing);
							snd_mgr->playSound(comp_w.p_sfx);

							e->get<CInput>()->fire_primary = false;
							comp_w.p_rounds_current--;
							comp_w.p_cooldown_current = comp_w.p_cooldown;
							comp_w.p_delay_current = comp_w.p_delay;
						}
						else {
							comp_w.p_delay_current -= app_conf->frame_dt * app_conf->game_speed;
						}
					}
					else if (e->get<CInput>()->fire_secondary && comp_w.s_rounds_current) {
						if (comp_w.s_delay_current <= 0) {
							play_data.ent_mgr.spawnEntity(comp_w.s_tag, comp_w.secondary, e, pos, Entity::STATE_RUN, facing);
							snd_mgr->playSound(comp_w.s_sfx);

							e->get<CInput>()->fire_secondary = false;
							comp_w.s_rounds_current--;
							comp_w.s_cooldown_current = comp_w.s_cooldown;
							comp_w.s_delay_current = comp_w.s_delay;
						}
						else {
							comp_w.s_delay_current -= app_conf->frame_dt * app_conf->game_speed;
						}
					}
				}
			}
		}
	}
};

#endif