#ifndef WC_BOX
#define WC_BOX

#include <SFML/Graphics.hpp>
#include "Border.h"

class WCBox:public sf::Drawable, public sf::Transformable {
private:
    sf::Sprite* background;
	sf::Vector2i bg_offset;
	Border* border;
    sf::Vector2i size;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    void setBackground(sf::Texture& tex, int offset);
    void setBackground(sf::Sprite& sprite, sf::Vector2i offset);
    void setBackgroundColor(sf::Color color);
    void setBorder(Border& b);
    void setSize(sf::Vector2i s);
    void setPosition(float x, float y);


    WCBox();
    WCBox(const WCBox& wcb);
    ~WCBox();
};

#endif