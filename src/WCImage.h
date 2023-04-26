#ifndef WC_IMAGE
#define WC_IMAGE

#include <SFML/Graphics.hpp>

class WCImage:public sf::Drawable {
private:
    sf::Sprite* image;
    sf::Vector2i size;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    void setImage(sf::Sprite& sprite);
    void setPosition(float x, float y);
    void setOrigin(float x, float y);
	void setColor(sf::Color& color);
    sf::FloatRect getLocalBounds();
    sf::Vector2f getPosition();
    sf::Vector2i getSize();
	sf::Color getColor();

    WCImage();
    WCImage(const WCImage& wcb);
    ~WCImage();
};

#endif