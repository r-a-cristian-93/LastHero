#include "Border.h"
#include <iostream>

Border::Border() {}

Border::Border(const Border& b)
	:sprites(b.sprites)
	{}

void Border::setSprite(size_t name, sf::Sprite sprite) {
	sprites[name] = sprite;
}

sf::Sprite& Border::getSprite(size_t name) {
	return sprites[name];
}

void Border::setTexture(size_t name, sf::Texture texture) {
	textures[name] = texture;
}

sf::Texture& Border::getTexture(size_t name) {
	return textures[name];
}
