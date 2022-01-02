#ifndef SETTINGS
#define SETTINGS

#include <SFML/Graphics.hpp>

class Settings {
public:
	int w_width;
	int w_height;
	int font_size;
	sf::Font font;
	sf::Color font_color;
};

extern Settings* Conf;

#endif


