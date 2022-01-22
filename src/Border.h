#ifndef BORDER
#define BORDER

#include <SFML/Graphics.hpp>

class Border {
	std::map<size_t, sf::Sprite> sprites;
	std::map<size_t, sf::Texture> textures;

	sf::Sprite& top_left = sprites[Border::TOP_LEFT];
	sf::Sprite& top_center = sprites[Border::TOP_CENTER];
	sf::Sprite& top_right = sprites[Border::TOP_RIGHT];
	sf::Sprite& bottom_left = sprites[Border::BOTTOM_LEFT];
	sf::Sprite& bottom_center = sprites[Border::BOTTOM_CENTER];
	sf::Sprite& bottom_right = sprites[Border::BOTTOM_RIGHT];
	sf::Sprite& middle_left = sprites[Border::MIDDLE_LEFT];
	sf::Sprite& middle_right = sprites[Border::MIDDLE_RIGHT];

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
		MIDDLE_RIGHT
	};

	void setSprite(size_t name, sf::Sprite sprite);
	void setTexture(size_t name, sf::Texture texture);
	std::vector<sf::Sprite*> getSprites();
	sf::Sprite& getSprite(size_t name);
	sf::Texture& getTexture(size_t name);

	void match(const sf::IntRect& rect);

	Border();
	Border(const Border& b);
};

#endif
