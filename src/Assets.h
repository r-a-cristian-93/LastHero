#ifndef ASSETS
#define ASSETS

#include <vector>
#include <string>
#include <fstream>
#include "Entity.h"
#include "Border.h"

struct EntityDataset {
	size_t type;
	size_t id;
	int radius;
	int velocity;
	int fill_r;
	int fill_g;
	int fill_b;
	int out_r;
	int out_g;
	int out_b;
	int out_thk;
	size_t vertices;
	int lifespan;
} ;

class Assets {
private:
	std::ifstream file;
	std::string word;

	Components recipe_player;
	Components recipe_bullet;
	Components recipe_missle;
	std::map<int, Components> recipe_enemy;

	std::map<size_t, sf::Font> fonts;
	std::map<size_t, sf::Texture> textures;
	std::map<size_t, sf::Sprite> sprites;
	std::map<size_t, Border> borders;

	void loadEntities();
	void loadFonts();
	void loadGUI();

	void loadEntity();
	void loadTexture();
	void loadSprite();
	void loadBorders();
	void loadBorderRepeatable(size_t id, size_t texture_name, size_t sprite_name);
	sf::IntRect loadRect();
	sf::Texture makeRepeatable(const sf::Texture& original, sf::IntRect& rect);

public:
	enum {
		NONE,
		FONT_COURIER,
		FONT_MILITARY,
		TEX_GUI,
		TEX_FILL_DARK_GREEN,
		BORDER_SLICK,
		BORDER_THICK,
		SPRITE_ICON_SKULL,
		SPRITE_ICON_HART
	};

	Assets();

	template<class T>
	const T* get(Components& c) {
		return std::get<T*>(c);
	}

	Components& getRecipePlayer();
	Components& getRecipeBullet();
	Components& getRecipeMissle();
	Components& getRecipeEnemy(size_t recipe_id);
	sf::Font& getFont(size_t name);
	Border& getBorder(size_t name);
	sf::Texture& getTexture(size_t name);
	sf::Sprite& getSprite(size_t name);

};

#endif
