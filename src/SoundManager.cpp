#include "SoundManager.h"
#include "SharedResources.h"
#include <iostream>

SoundManager::SoundManager()
	:bg_music(nullptr)
	{}

SoundManager::~SoundManager() {
	for (int i=0; i<channels.size(); i++) {
		channels[i].stop();
	}

	channels.clear();

	if (bg_music) bg_music->stop();
}

bool SoundManager::isPlaying(size_t id) {
	const sf::SoundBuffer* sb = &assets->getSoundBuffer(id);

	for (sf::Sound& ch: channels) {
		if (ch.getBuffer() == sb) {
			if (ch.getStatus() == sf::SoundSource::Status::Playing) {
				return true;
			}
		}
	}

	return false;
}

void SoundManager::playSoundUnique(size_t id) {
	if (!isPlaying(id)) {
		playSound(id);
	}
}

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
	channels.back().setVolume(app_conf->sfx_volume);
	channels.back().play();
}

void SoundManager::playSound(std::string name) {
	size_t id = assets->getSoundBufferNameID(name);

	playSound(id);
}

void SoundManager::playBgMusic(std::string name) {
	if (bg_music == &assets->getSound(name)) {
		if (bg_music->getStatus() != sf::SoundSource::Status::Playing) {
			bg_music->setVolume(app_conf->music_volume);
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
		bg_music->setVolume(app_conf->music_volume);
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

bool SoundManager::bgPlaying() {
	if (bg_music)
		if (bg_music->getStatus() == sf::SoundSource::Status::Playing)
			return true;

	return false;
}


