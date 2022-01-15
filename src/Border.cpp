#include "Border.h"
#include <iostream>

Border::Border() {}

Border::Border(const Border& b)
	:sprites(b.sprites)
	{}

void Border::setSprite(size_t place, sf::Sprite sprite) {
	std::cout << "add border " << place << std::endl;
	sprites[place] = sprite;
}

sf::Sprite& Border::getSprite(size_t name) {
	return sprites[name];
}

