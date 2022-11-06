#include "WCImage.h"
#include <iostream>

WCImage::WCImage()
    :image(nullptr)
    {}

WCImage::WCImage(const WCImage& wcb)
    :image(nullptr)
{
    if (wcb.image) setImage(*wcb.image);
}

WCImage::~WCImage() {
    if (image) delete image;
}

void WCImage::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (image != nullptr) {
        target.draw(*image);
    }
}

void WCImage::setImage(sf::Sprite& sprite) {
	if (image == nullptr) {
		image = new sf::Sprite(sprite);
	}
}

void WCImage::setOrigin(float x, float y) {
	if (image!= nullptr) {
		image->setOrigin(x, y);
	}
}

void WCImage::setPosition(float x, float y) {
    if (image != nullptr) {
        image->setPosition(x, y);
    }
}

sf::FloatRect WCImage::getLocalBounds() {
	if (image != nullptr) {
		return image->getLocalBounds();
	}
	else {
		return {0.0f,0.0f,0.0f,0.0f};
	}
}
