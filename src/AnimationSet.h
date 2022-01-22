#ifndef ANIMATION_SET
#define ANIMATION_SET

#include "Animation.h"

class AnimationSet {
public:
	enum {
		ANIM_SPAWN,
		ANIM_STANCE,
		ANIM_MOVE_LEFT,
		ANIM_MOVE_RIGHT,
		ANIM_FIRE_PRIMARY,
		ANIM_FIRE_SECONDARY,
		ANIM_HIT,
		ANIM_DIE
	};

	std::map<size_t, Animation> animations;

	AnimationSet();
	AnimationSet(const AnimationSet& set);
};

#endif
