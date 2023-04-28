#ifndef SYSTEM_STATE_FACING_H
#define SYSTEM_STATE_FACING_H

#include "System.h"
#include "ScenePlayData.h"

class SystemStateFacing: public System {
private:
	ScenePlayData& play_data;

public:
	SystemStateFacing(ScenePlayData& _play_data): play_data(_play_data) {}

	void operator() () {
		for (std::shared_ptr<Entity>& e : play_data.ent_mgr.getEntities()) {

			// update facing direction;
			if (!e->blocked) {
				sf::Vector2f dir(e->get<CTransform>()->prev_dir);
				if (dir.x == 1 && dir.y == 0) e->facing = Entity::FACING_E;
				if (dir.x == 1 && dir.y == -1) e->facing = Entity::FACING_NE;
				if (dir.x == 0 && dir.y == -1) e->facing = Entity::FACING_N;
				if (dir.x == -1 && dir.y == -1) e->facing = Entity::FACING_NW;
				if (dir.x == -1 && dir.y == 0) e->facing = Entity::FACING_W;
				if (dir.x == -1 && dir.y == 1) e->facing = Entity::FACING_SW;
				if (dir.x == 0 && dir.y == 1) e->facing = Entity::FACING_S;
				if (dir.x == 1 && dir.y == 1) e->facing = Entity::FACING_SE;
			}

			//change state;
			if (!e->blocked) {
				if (e->get<CTransform>()) {
					if (e->get<CTransform>()->vel.x || e->get<CTransform>()->vel.y) {
						e->state = Entity::STATE_RUN;
					}
					else {
						e->state = Entity::STATE_IDLE;
					}
				}

				if (e->hit && e->get<CAnimation>()->anim_set.animations[Entity::STATE_HIT].count(e->facing) != 0) {
					e->state = Entity::STATE_HIT;
					e->blocked = true;
					e->hit = false;
				}

				// block when firing only if it has firing animation
				if (e->get<CInput>() && e->get<CWeapon>()) {
					CWeapon& comp_w = *e->get<CWeapon>();

					if (e->get<CInput>()->fire_primary && comp_w.p_cooldown_current == 0 && comp_w.p_rounds_current) {
						if (e->get<CAnimation>()->anim_set.animations[Entity::STATE_FIRE_PRIMARY].count(e->facing) != 0) {
							e->state = Entity::STATE_FIRE_PRIMARY;
							e->blocked = true;
						}
					}
					else if (e->get<CInput>()->fire_secondary && comp_w.s_cooldown_current == 0 && comp_w.s_rounds_current) {
						if (e->get<CAnimation>()->anim_set.animations[Entity::STATE_FIRE_SECONDARY].count(e->facing) != 0) {
							e->state = Entity::STATE_FIRE_SECONDARY;
							e->blocked = true;
						}
					}
				}
			}
			//if blocking animation has ended
			else if (e->get<CAnimation>()->active_anim->hasEnded()) {
				e->blocked = false;
				e->state = Entity::STATE_IDLE;
			}

			if (!e->alive) {
				if (e->blocked && e->state != Entity::STATE_DIE) {
					e->get<CAnimation>()->active_anim->has_ended = true;
				}

				e->state = Entity::STATE_DIE;
			}
		}
	}
};

#endif