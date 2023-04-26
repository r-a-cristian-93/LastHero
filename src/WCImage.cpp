#include "WCImage.h"
#include <iostream>

WCImage::WCImage()
    :image(nullptr)
    {}

WCImage::WCImage(const WCImage& wcb)
    :image(nullptr)
	,size(wcb.size)
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
		sf::FloatRect bounds = image->getLocalBounds();
		size.x = bounds.width;
		size.y = bounds.height;
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

void WCImage::setColor(sf::Color& color) {
	if (image != nullptr) {
		image->setColor(color);
	}
}

sf::Vector2f WCImage::getPosition() {
	if (image != nullptr) {
		return image->getPosition();
	}
	else {
		return {0.0f, 0.0f};
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

sf::Vector2i WCImage::getSize() {
	return size;
}

sf::Color WCImage::getColor() {
	if (image != nullptr) {
		return image->getColor();
	}
	else {
		return {};
	}
}
