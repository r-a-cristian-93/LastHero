#ifndef BOX
#define BOX

#include <SFML/Graphics.hpp>

class Box {
	std::map<size_t, sf::Sprite> sprites;
	std::map<size_t, sf::Texture> textures;

	sf::Sprite& top_left = sprites[Box::TOP_LEFT];
	sf::Sprite& top_center = sprites[Box::TOP_CENTER];
	sf::Sprite& top_right = sprites[Box::TOP_RIGHT];
	sf::Sprite& bottom_left = sprites[Box::BOTTOM_LEFT];
	sf::Sprite& bottom_center = sprites[Box::BOTTOM_CENTER];
	sf::Sprite& bottom_right = sprites[Box::BOTTOM_RIGHT];
	sf::Sprite& middle_left = sprites[Box::MIDDLE_LEFT];
	sf::Sprite& middle_right = sprites[Box::MIDDLE_RIGHT];
	sf::Sprite& middle_center = sprites[Box::MIDDLE_CENTER];

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
		MIDDLE_CENTER
	};

	void setSprite(size_t name, sf::Sprite sprite);
	void setTexture(size_t name, sf::Texture texture);
	std::vector<sf::Sprite*> getSprites();
	sf::Sprite& getSprite(size_t name);
	sf::Texture& getTexture(size_t name);

	void match(const sf::IntRect& rect);

	Box();
	Box(const Box& b);
};

#endif
