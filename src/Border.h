#ifndef BORDER
#define BORDER

#include <SFML/Graphics.hpp>

class Border {
	std::map<std::string, sf::Sprite> sprites;
	std::map<std::string, sf::Texture> textures;

	sf::Sprite& top_left = sprites["top_left"];
	sf::Sprite& top_center = sprites["top_center"];
	sf::Sprite& top_right = sprites["top_right"];
	sf::Sprite& bottom_left = sprites["bottom_left"];
	sf::Sprite& bottom_center = sprites["bottom_center"];
	sf::Sprite& bottom_right = sprites["bottom_right"];
	sf::Sprite& middle_left = sprites["middle_left"];
	sf::Sprite& middle_right = sprites["middle_right"];

public:
	void setSprite(std::string name, sf::Sprite sprite);
	void setTexture(std::string name, sf::Texture texture);
	std::vector<sf::Sprite*> getSprites();
	sf::Sprite& getSprite(std::string name);
	sf::Texture& getTexture(std::string name);

	void match(const sf::IntRect& rect);

	Border();
	Border(const Border& b);
};

#endif
