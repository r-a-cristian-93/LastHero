#include "Animation.h"
#include <iostream>

Animation::Animation()
	:total_frames(0)
	,frames_alive(0)
	,frames_changed(0)
	,active_frame(0)
	,play(0)
	,has_ended(false)
	{}

Animation::Animation(std::vector<sf::Sprite> _sprites, std::vector<size_t> _frame_time, size_t _play)
	:total_frames(0)
	,frames_alive(0)
	,frames_changed(0)
	,active_frame(0)
	,play(_play)
	,has_ended(true)
	,sprites(_sprites)
	,frame_time(_frame_time)
{
	for (int i=0; i<frame_time.size(); i++) {
		total_frames += frame_time[i];
	}
}

void Animation::update() {
	frames_alive++;

	if (play == PLAY_LOOP) {
		if (frames_alive > frames_changed + frame_time[active_frame]) {
			active_frame++;
			frames_changed = frames_alive;

			if (active_frame >= frame_time.size()) active_frame = 0;
		}
	}
	else if (play == PLAY_ONCE) {

	}
}

sf::Sprite& Animation::getSprite() {
	return sprites[active_frame];
}

bool Animation::hasEnded() {
	return has_ended;
}
