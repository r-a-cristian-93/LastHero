#ifndef ANIMATION_SET
#define ANIMATION_SET

#include "Animation.h"

class AnimationSet {
public:
	enum {
		ASET_SPAWN,
		ASET_STANCE,
		ASET_MOVE_LEFT,
		ASET_MOVE_RIGHT,
		ASET_FIRE_PRIMARY,
		ASET_FIRE_SECONDARY,
		ASET_HIT,
		ASET_DIE
	};

	std::map<size_t, Animation> animations;

	AnimationSet();
	AnimationSet(const AnimationSet& set);
};

#endif
