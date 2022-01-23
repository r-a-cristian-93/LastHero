#include "CAnimation.h"

CAnimation::CAnimation() {}

CAnimation::CAnimation(const CAnimation& c)
	:anim_set(c.anim_set)
{
	active_anim = &anim_set.animations[0];
}

CAnimation::CAnimation(const AnimationSet& a)
	:anim_set(a)
{
	active_anim = &anim_set.animations[0];
}
