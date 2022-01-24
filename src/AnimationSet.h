#ifndef ANIMATION_SET
#define ANIMATION_SET

#include "Animation.h"

class AnimationSet {
public:
	enum {
		ANIM_SPAWN,
		ANIM_STANCE,
		ANIM_MOVE_N,
		ANIM_MOVE_S,
		ANIM_MOVE_E,
		ANIM_MOVE_W,
		ANIM_MOVE_NE,
		ANIM_MOVE_NW,
		ANIM_MOVE_SE,
		ANIM_MOVE_SW,
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
