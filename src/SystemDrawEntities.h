#ifndef SYSTEM_DRAW_ENTITIES
#define SYSTEM_DRAW_ENTITIES

#include <algorithm>
#include "System.h"
#include "ScenePlayData.h"
#include "SharedResources.h"

class SystemDrawEntities: public System {
private:
	ScenePlayData& play_data;
	EntityVec& entities = play_data.ent_mgr.getEntities();

public:
	SystemDrawEntities(ScenePlayData& _play_data)
		:play_data(_play_data)
	{}

	void operator() () {
		std::sort(entities.begin(), entities.end(), comparePosition);

		for (const std::shared_ptr<Entity>& e:entities) {
			if (e->get<CAnimation>()) {
				screen_tex->draw(e->get<CAnimation>()->active_anim->getSprite());
			}

			#ifdef DRAW_COLLISION_BOX
			if (e->get<CShape>() && e->get<CCollision>()) {
				sf::Vector2f& pos = e->get<CTransform>()->pos;

				for (HitBox& hb : e->get<CCollision>()->hitbox) {
					e->get<CShape>()->shape.setRadius(hb.radius);
					e->get<CShape>()->shape.setOrigin(hb.radius, hb.radius);
					e->get<CShape>()->shape.setPosition(pos + hb.offset[e->facing]);
					w->draw(e->get<CShape>()->shape);
				}
			}
			#endif
		}
	}

private:
	static bool comparePosition(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
		if (a->get<CAnimation>() && b->get<CAnimation>()) {
			if (a->get<CAnimation>()->prio != b->get<CAnimation>()->prio) {
				return (a->get<CAnimation>()->prio < b->get<CAnimation>()->prio);
			}
		}

		if (a->get<CTransform>() && b->get<CTransform>()) {
			return (a->get<CTransform>()->pos.y < b->get<CTransform>()->pos.y);
		}

		return false;
	}
};

#endif