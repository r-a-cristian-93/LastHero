#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager()
	:assets(nullptr)
	{}

SoundManager::SoundManager(Assets* _assets)
	:assets(_assets)
	{}

void SoundManager::playSound(size_t id) {
	if (channels.size() >= MAX_CHANNELS) {
		std::vector<sf::Sound>::iterator it = channels.begin();

		while (it != channels.end()) {
			if (it->getStatus() != sf::SoundSource::Status::Playing) {
				it = channels.erase(it);
			}
			else {
				it--;
			}
		}
	}

	channels.push_back(sf::Sound(assets->getSoundBuffer(id)));
	channels.back().play();
}

void SoundManager::playSound(std::string name) {
	size_t id = assets->getSoundBufferNameID(name);

	playSound(id);
}
