#ifndef ANIMATION_SET
#define ANIMATION_SET

#include "Animation.h"

class AnimationSet {
public:
	std::map<size_t, std::map<size_t, Animation>> animations;

	AnimationSet();
	AnimationSet(const AnimationSet& set);
};

#endif
