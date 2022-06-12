#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager()
	:assets(nullptr)
	,bg_music(nullptr)
	{}

SoundManager::SoundManager(Assets* _assets)
	:assets(_assets)
	,bg_music(nullptr)
	{}

void SoundManager::playSound(size_t id) {
	if (!id) return;

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

void SoundManager::playBgMusic(std::string name) {
	if (bg_music == &assets->getSound(name)) {
		if (bg_music->getStatus() != sf::SoundSource::Status::Playing) {
			bg_music->setVolume(100);
			bg_music->play();
		}

		return;
	}
	else {
		if (bg_music) {
			bg_music->stop();
			bg_music = nullptr;
		}

		bg_music = &assets->getSound(name);
		bg_music->setVolume(100);
		bg_music->play();
	}
}

void SoundManager::setBgMusicVolume(float v) {
	if (bg_music) bg_music->setVolume(v);
}

void SoundManager::stopBgMusic() {
	if (bg_music) bg_music->stop();
}

void SoundManager::playBgMusic() {
	if (bg_music) bg_music->play();
}

void SoundManager::pauseBgMusic() {
	if (bg_music) bg_music->pause();
}


