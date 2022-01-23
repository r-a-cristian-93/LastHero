#include "Animation.h"
#include <iostream>

Animation::Animation()
	:frames_alive(0)
	,active_frame(0)
	,speed(0)
	,play(0)
	,has_ended(false)
	{}

Animation::Animation(std::vector<sf::Sprite> _sprites, size_t _speed, size_t _play)
	:frames_alive(0)
	,active_frame(0)
	,speed(_speed)
	,play(_play)
	,has_ended(false)
	,sprites(_sprites)
{
	frames = sprites.size();
}

void Animation::update() {
	frames_alive++;

	if (play == PLAY_LOOP) {
		active_frame = (frames_alive / speed) % frames;
	}
	else if (play == PLAY_ONCE) {
		has_ended = false;
		if (frames_alive >= frames * speed) {
			has_ended = true;
			frames_alive = 0;
		}
		active_frame = (frames_alive / speed) % frames;
	}
}

sf::Sprite& Animation::getSprite() {
	return sprites[active_frame];
}

bool Animation::hasEnded() {
	return has_ended;
}
