#include "FrameManager.h"
#include "SharedResources.h"

FrameManager::FrameManager() {}

void FrameManager::update() {
	frame_dt = clock.restart().asSeconds();
	frame_increment = frame_dt * app_conf->game_speed;
}

const float& FrameManager::getIncrement() {
	return frame_increment;
}