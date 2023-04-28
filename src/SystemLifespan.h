#ifndef SYSTEM_LIFE_SPAN_H
#define SYSTEM_LIFE_SPAN_H

#include "System.h"

class SystemLifespan: public System {
public:
	using System::System;

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
			const int lifespan = e->get<CLifespan>()->lifespan;
			int& remaining = e->get<CLifespan>()->remaining;

			remaining--;

			if (remaining <= 0) {
				e->alive = false;
			}
		}
	}
};

#endif