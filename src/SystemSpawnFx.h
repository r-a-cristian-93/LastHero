#ifndef SYSTEM_SPAWN_FX_H
#define SYSTEM_SPAWN_FX_H

#include "System.h"
#include "ScenePlayData.h"

class SystemSpawnFx: public System {
private:
	ScenePlayData& play_data;

public:
	SystemSpawnFx(ScenePlayData& _play_data): play_data(_play_data) {}

	void operator() () {
		for (std::shared_ptr<Entity>& e : play_data.ent_mgr.getEntities(TAG::PROJECTILE)) {
			if (e->get<CAnimation>() && e->get<CFx>()) {
				for (Fx fx : e->get<CFx>()->fx) {
					switch (fx.trigger) {
						case TR::DIE:
							if (!e->alive && e->get<CAnimation>()->active_anim->hasEnded()) {
								play_data.ent_mgr.spawnEntity(fx.tag, fx.id, e->get<CTransform>()->pos, Entity::STATE_DIE, Entity::FACING_E);
							}
						break;
					}
				}
			}
		}
	}
};

#endif