#ifndef BORDER
#define BORDER

#include <SFML/Graphics.hpp>

typedef std::map<size_t, sf::Sprite> SpriteMap;

class Border {
	SpriteMap sprites;

public:
	enum {
		NONE,
		TOP_LEFT,
		TOP_CENTER,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_CENTER,
		BOTTOM_RIGHT,
		MIDDLE_LEFT,
		MIDDLE_RIGHT,
	};

	void setSprite(size_t place, sf::Sprite sprite);
	sf::Sprite& getSprite(size_t name);

	Border();
	Border(const Border& b);
};

#endif
