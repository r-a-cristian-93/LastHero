#include "CAnimation.h"

CAnimation::CAnimation() {}

CAnimation::CAnimation(const CAnimation& c)
	:anim_set(c.anim_set)
	,prio(c.prio)
	,outline_render_states(c.outline_render_states.shader)
{
	const size_t state = anim_set.animations.begin()->first;
	active_anim = &anim_set.animations[state].begin()->second;
}

CAnimation::CAnimation(const AnimationSet& a)
	:anim_set(a)
{
	const size_t state = anim_set.animations.begin()->first;
	active_anim = &anim_set.animations[state].begin()->second;
}
