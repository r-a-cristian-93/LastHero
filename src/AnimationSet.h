#ifndef ANIMATION_SET
#define ANIMATION_SET

#include "Animation.h"

typedef std::map<size_t, std::map<size_t, Animation>> AnimMapState;
typedef std::map<size_t, Animation> AnimMapDirection;

class AnimationSet {
public:
	AnimMapState animations;
	sf::Color color_mod;

	AnimationSet();
	AnimationSet(const AnimationSet& set);

	void setColorMod(sf::Color color);
};

#endif
