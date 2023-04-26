#include "WCBox.h"
#include <iostream>

WCBox::WCBox()
	:box(nullptr)
    {}

WCBox::WCBox(const WCBox& wcb)
    :sf::Transformable(wcb)
    ,box(nullptr)
	,size(wcb.size)
{
	if (wcb.box) setStyle(*wcb.box);
}

WCBox::~WCBox() {
    if (box) delete box;
}

void WCBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (box != nullptr) {
		for (sf::Sprite* s : box->getSprites()) {
			target.draw(*s, states);
		}
	}
}

void WCBox::setPosition(float x, float y) {
    sf::Transformable::setPosition(x,y);

    if (box != nullptr) {

	}
}

void WCBox::setStyle(Box& b) {
	if (!box) {
		box = new Box(b);

		box->match(sf::IntRect(this->getPosition().x, this->getPosition().y, size.x, size.y));
	}
}

sf::Vector2i WCBox::getSize() {
	return size;
}

void WCBox::setSize(sf::Vector2i s) {
	size = s;
}

sf::FloatRect WCBox::getGlobalBounds() {
	return {0, 0, size.x, size.y};
}
