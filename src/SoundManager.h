#ifndef SOUND_MANAGER
#define SOUND_MANAGER

#include "Common.h"
#include "Assets.h"
#include "AppConfig.h"

#define MAX_CHANNELS 32

class SoundManager {
private:
	Assets* assets;
	AppConfig* app_conf;
	std::vector<sf::Sound> channels;
	sf::Music* bg_music;

public:
	void playSound(size_t id);
	void playSound(std::string name);

	void playBgMusic(std::string name);
	void playBgMusic();
	void pauseBgMusic();
	void stopBgMusic();
	void setBgMusicVolume(float v);
	bool bgPlaying();

	SoundManager();
	SoundManager(Assets* assets, AppConfig* config);
};

#endif
