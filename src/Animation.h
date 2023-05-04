#ifndef ANIMATION
#define ANIMATION

#include <SFML/Graphics.hpp>

typedef std::vector<sf::Sprite> SpritesVec;

class Animation {
public:
	SpritesVec sprites;
	std::vector<size_t> frame_time;
	size_t total_frames;
	float frames_alive, frames_changed, active_frame;
	size_t play;
	bool has_ended;

	enum {
		NONE = 0,
		PLAY_ONCE,
		PLAY_LOOP,
		PLAY_SWING,
	};

	Animation();
	Animation(std::vector<sf::Sprite> sprites, std::vector<size_t> frame_time, size_t play);

	void reset();
	void update();
	bool hasEnded();
	sf::Sprite& getSprite();
};

#endif
