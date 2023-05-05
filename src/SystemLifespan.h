#ifndef SYSTEM_LIFE_SPAN_H
#define SYSTEM_LIFE_SPAN_H

#include "System.h"
#include "ScenePlayData.h"

class SystemLifespan: public System {
private:
	ScenePlayData& play_data;

public:
	SystemLifespan(ScenePlayData& _play_data): play_data(_play_data) {}

	void operator() () {
		for (std::shared_ptr<Entity>& e : play_data.ent_mgr.getEntities(TAG::PROJECTILE)) {
			checkLifespan(e);
		}

		for (std::shared_ptr<Entity>& e : play_data.ent_mgr.getEntities(TAG::FX)) {
			checkLifespan(e);
		}
	}

private:
	void checkLifespan(std::shared_ptr<Entity>& e) {
		if (e->get<CLifespan>()) {
			const float lifespan = e->get<CLifespan>()->lifespan;
			float& remaining = e->get<CLifespan>()->remaining;

			remaining -= frame_mgr->getIncrement();;

			if (remaining <= 0) {
				e->alive = false;
			}
		}
	}
};

#endif