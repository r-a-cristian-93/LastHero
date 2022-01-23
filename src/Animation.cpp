#include "Animation.h"

Animation::Animation()
	:frames_alive(0)
	,active_frame(0)
	,speed(0)
	,play(0)
	{}

Animation::Animation(std::vector<sf::Sprite> _sprites, size_t _speed, size_t _play)
	:frames_alive(0)
	,active_frame(0)
	,speed(_speed)
	,play(_play)
	,sprites(_sprites)
{
	frames = sprites.size();
}

void Animation::update() {
	frames_alive++;
	active_frame = (frames_alive / speed) % frames;
}

sf::Sprite& Animation::getSprite() {
	return sprites[active_frame];
}
