#include "CAnimation.h"

CAnimation::CAnimation() {}

CAnimation::CAnimation(const CAnimation& c)
	:anim_set(c.anim_set)
{
	const size_t state = anim_set.animations.begin()->first;
	active_anim = &anim_set.animations[state][1];
}

CAnimation::CAnimation(const AnimationSet& a)
	:anim_set(a)
{
	const size_t state = anim_set.animations.begin()->first;
	active_anim = &anim_set.animations[state][1];
}
