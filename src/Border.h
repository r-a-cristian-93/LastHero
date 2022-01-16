#ifndef BORDER
#define BORDER

#include <SFML/Graphics.hpp>

class Border {
	std::map<size_t, sf::Sprite> sprites;
	std::map<size_t, sf::Texture> textures;

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
		TEX_TOP_CENTER,
		TEX_BOTTOM_CENTER,
		TEX_MIDDLE_LEFT,
		TEX_MIDDLE_RIGHT
	};

	void setSprite(size_t name, sf::Sprite sprite);
	void setTexture(size_t name, sf::Texture texture);
	sf::Sprite& getSprite(size_t name);
	sf::Texture& getTexture(size_t name);

	Border();
	Border(const Border& b);
};

#endif
