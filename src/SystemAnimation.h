#ifndef SYSTEM_ANIMATION_H
#define SYSTEM_ANIMATION_H

#include "System.h"

class SystemAnimation: public System {
public:
	using System::System;

	void operator() () {
		for (std::shared_ptr<Entity>& e : play_data.ent_mgr.getEntities()) {
			if (e->get<CAnimation>() && e->facing != 0) {

				const size_t state = e->state;
				size_t& facing = e->facing;
				AnimMapState& animations = e->get<CAnimation>()->anim_set.animations;
				Animation*& active_anim = e->get<CAnimation>()->active_anim;
				size_t has_state_animation = animations.count(state);

				active_anim->update();

				if ((active_anim->hasEnded() || active_anim->play == Animation::PLAY_LOOP)) {
					if (has_state_animation != 0) {
						if (animations[state].count(facing) == 0) {
							facing = animations[state].begin()->first;
						}

						if (active_anim != &animations[state][facing]) {
							// set new animation
							active_anim = &animations[state][facing];
							active_anim->reset();
						}
					}
				}

				// update animation sprite position
				if (e->get<CTransform>()) {
					active_anim->getSprite().setPosition(e->get<CTransform>()->pos);
				}
			}
		}
	}
};

#endif