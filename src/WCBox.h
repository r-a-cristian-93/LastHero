#ifndef WC_BOX
#define WC_BOX

#include <SFML/Graphics.hpp>
#include "Box.h"

class WCBox:public sf::Drawable, public sf::Transformable {
private:
	Box* box;
    sf::Vector2i size;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    void setStyle(Box& b);
    void setSize(sf::Vector2i s);
    void setPosition(float x, float y);
    sf::Vector2i getSize();
    sf::FloatRect getGlobalBounds();


    WCBox();
    WCBox(const WCBox& wcb);
    ~WCBox();
};

#endif