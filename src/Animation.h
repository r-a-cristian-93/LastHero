#ifndef ANIMATION
#define ANIMATION

#include <SFML/Graphics.hpp>

class Animation {
	std::vector<sf::Sprite> sprites;
	size_t frames, frames_alive, instant_frame;
	size_t speed;
	size_t type;

public:
	enum {
		NONE,
		ANIM_ONE,
		ANIM_LOOP,
		ANIM_SWING,
	};

	Animation(std::vector<sf::Sprite> sprites, size_t speed, size_t type);

	void update();
	bool hasEnded();
	sf::Sprite& getSprite();
};

#endif
