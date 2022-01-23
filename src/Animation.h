#ifndef ANIMATION
#define ANIMATION

#include <SFML/Graphics.hpp>

class Animation {
public:
	std::vector<sf::Sprite> sprites;
	size_t frames, frames_alive, active_frame;
	size_t speed;
	size_t play;
	bool has_ended;

	enum {
		NONE,
		PLAY_ONCE,
		PLAY_LOOP,
		PLAY_SWING,
	};

	Animation();
	Animation(std::vector<sf::Sprite> sprites, size_t speed, size_t play);

	void update();
	bool hasEnded();
	sf::Sprite& getSprite();
};

#endif
