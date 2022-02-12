#ifndef ANIMATION
#define ANIMATION

#include <SFML/Graphics.hpp>

class Animation {
public:
	std::vector<sf::Sprite> sprites;
	std::vector<size_t> frame_time;
	size_t total_frames, frames_alive, frames_changed, active_frame, previous_acrive_frame;
	size_t play;
	bool has_ended;

	enum {
		NONE,
		PLAY_ONCE,
		PLAY_LOOP,
		PLAY_SWING,
	};

	Animation();
	Animation(std::vector<sf::Sprite> sprites, std::vector<size_t> frame_time, size_t play);

	void update();
	bool hasEnded();
	sf::Sprite& getSprite();
};

#endif
