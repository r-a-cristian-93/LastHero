#include "AnimationSet.h"

AnimationSet::AnimationSet() {}

AnimationSet::AnimationSet(const AnimationSet& set)
	:animations(set.animations)
	,color_mod(set.color_mod)
	{}

void AnimationSet::setColorMod(sf::Color color) {
	color_mod = color;

	AnimMapState::iterator state_it;
	for (state_it = animations.begin(); state_it != animations.end(); state_it++) {
		AnimMapDirection::iterator dir_it;

		for (dir_it = state_it->second.begin(); dir_it != state_it->second.end(); dir_it++) {
			SpritesVec& sprites_vec = dir_it->second.sprites;

			for (sf::Sprite& sprite : sprites_vec) {
				sprite.setColor(color);
			}
		}
	}
}
