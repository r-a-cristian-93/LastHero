#include "Border.h"
#include <iostream>

Border::Border() {}

Border::Border(const Border& b)
	:sprites(b.sprites)
	{}

void Border::setSprite(std::string name, sf::Sprite sprite) {
	sprites[name] = sprite;
}

sf::Sprite& Border::getSprite(std::string name) {
	return sprites[name];
}

void Border::setTexture(std::string name, sf::Texture texture) {
	textures[name] = texture;
}

sf::Texture& Border::getTexture(std::string name) {
	return textures[name];
}

void Border::match(const sf::IntRect& rect) {
	top_left.setPosition(rect.left, rect.top);
	top_center.setPosition(rect.left + top_left.getTextureRect().width, rect.top);
	top_right.setPosition(rect.left + rect.width - top_right.getTextureRect().width, rect.top);
	bottom_left.setPosition(rect.left, rect.top + rect.height - bottom_left.getTextureRect().height);
	bottom_center.setPosition(rect.left + bottom_left.getTextureRect().width, rect.top + rect.height - bottom_center.getTextureRect().height);
	bottom_right.setPosition(rect.left + rect.width - bottom_right.getTextureRect().width, rect.top + rect.height - bottom_right.getTextureRect().height);
	middle_left.setPosition(rect.left, rect.top + top_left.getTextureRect().height);
	middle_right.setPosition(rect.left + rect.width - middle_right.getTextureRect().width, rect.top + top_right.getTextureRect().height);

	top_center.setTextureRect(sf::IntRect(0,0, rect.width - top_left.getTextureRect().width - top_right.getTextureRect().width, top_center.getTextureRect().height));
	bottom_center.setTextureRect(sf::IntRect(0,0, rect.width - bottom_left.getTextureRect().width - bottom_right.getTextureRect().width, bottom_center.getTextureRect().height));
	middle_left.setTextureRect(sf::IntRect(0,0, middle_left.getTextureRect().width, rect.height - top_left.getTextureRect().height - bottom_left.getTextureRect().height));
	middle_right.setTextureRect(sf::IntRect(0,0, middle_right.getTextureRect().width, rect.height - top_right.getTextureRect().height - bottom_right.getTextureRect().height));
}

std::vector<sf::Sprite*> Border::getSprites() {
	std::vector<sf::Sprite*> sprites;
	sprites.push_back(&top_left);
	sprites.push_back(&top_center);
	sprites.push_back(&top_right);
	sprites.push_back(&bottom_left);
	sprites.push_back(&bottom_center);
	sprites.push_back(&bottom_right);
	sprites.push_back(&middle_left);
	sprites.push_back(&middle_right);
	return sprites;
}
