#ifndef WC_IMAGE
#define WC_IMAGE

#include <SFML/Graphics.hpp>

class WCImage:public sf::Drawable {
private:
    sf::Sprite* image;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    void setImage(sf::Sprite& sprite);
    void setPosition(float x, float y);
    void setOrigin(float x, float y);
    sf::FloatRect getLocalBounds();
    sf::Vector2f getPosition();
    sf::Vector2f getSize();

    WCImage();
    WCImage(const WCImage& wcb);
    ~WCImage();
};

#endif