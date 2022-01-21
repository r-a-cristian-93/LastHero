#include "Animation.h"

Animation::Animation(std::vector<sf::Sprite> spr, size_t speed, size_t t)
	:frames_alive(0)
	,speed(0)
	,type(t)
	,sprites(spr)
{
	frames = sprites.size();
}

void Animation::update() {
	frames_alive++;
	instant_frame = (frames_alive / speed) % frames;
}

sf::Sprite& Animation::getSprite() {
	return sprites[instant_frame];
}
