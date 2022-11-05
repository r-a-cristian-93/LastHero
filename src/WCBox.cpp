#include "WCBox.h"
#include <iostream>

WCBox::WCBox()
    :background(nullptr)
	,bg_offset(0,0)
	,border(nullptr)
    {}

WCBox::WCBox(const WCBox& wcb)
    :sf::Transformable(wcb)
    ,background(nullptr)
    ,bg_offset(0,0)
    ,border(nullptr)
{
    if (wcb.background) setBackground(*wcb.background, wcb.bg_offset);
	if (wcb.border) setBorder(*wcb.border);
}

WCBox::~WCBox() {
    if (background) delete background;
    if (border) delete border;
}

void WCBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(*background);
}

void WCBox::setBackground(sf::Texture& tex, int offset) {
	if (!background) {
		bg_offset = {offset, offset};
		background = new sf::Sprite(tex, sf::IntRect(0, 0, size.x-offset*2, size.y-offset*2));
		background->setPosition(this->getPosition().x+bg_offset.x, this->getPosition().y+bg_offset.y);
		//drawables.push_back(background);
	}
}

void WCBox::setBackground(sf::Sprite& sprite, sf::Vector2i offset) {
	if (background == nullptr) {
		bg_offset = offset;
		background = new sf::Sprite(sprite);
		background->setPosition(this->getPosition().x+bg_offset.x, this->getPosition().y+bg_offset.y);
		//drawables.push_back(background);
	}
}

void WCBox::setBackgroundColor(sf::Color color) {
	if (background) background->setColor(color);
}

void WCBox::setPosition(float x, float y) {
    sf::Transformable::setPosition(x,y);

    if (background != nullptr) {
        std::cout << "update bg pos " << x << "/" << y << std::endl;
        background->setPosition(x, y);
    }
}

void WCBox::setBorder(Border& b) {
	if (!border) {
		border = new Border(b);

		border->match(sf::IntRect(this->getPosition().x, this->getPosition().y, size.x, size.y));

		for (sf::Sprite* sprite:border->getSprites()) {
			//drawables.push_back(sprite);
		}
	}
}

void WCBox::setSize(sf::Vector2i s) {
	size = s;
}
