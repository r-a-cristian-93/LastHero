#ifndef C_ANIMATION
#define C_ANIMATION

#include "AnimationSet.h"

class CAnimation {
public:
	AnimationSet anim_set;
	Animation* active_anim;
	size_t prio;

	CAnimation();
	CAnimation(const CAnimation& c);
	CAnimation(const AnimationSet& a);
};

#endif
