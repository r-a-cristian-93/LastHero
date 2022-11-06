#include "WCBox.h"
#include <iostream>

WCBox::WCBox()
	:box(nullptr)
    {}

WCBox::WCBox(const WCBox& wcb)
    :sf::Transformable(wcb)
    ,box(nullptr)
{
	if (wcb.box) setBorder(*wcb.box);
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

void WCBox::setBorder(Box& b) {
	if (!box) {
		box = new Box(b);

		box->match(sf::IntRect(this->getPosition().x, this->getPosition().y, size.x, size.y));
	}
}

sf::Vector2f WCBox::getSize() {
	if (box!= nullptr) {
		return box->getSize();
	}
	else {
		return {0.0f,0.0f};
	}
}

void WCBox::setSize(sf::Vector2i s) {
	size = s;
}
