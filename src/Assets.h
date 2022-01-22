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
	int experience;
	int level;
	int stats_base[CStats::COUNT];
	int stats_per_level[CStats::COUNT];
} ;

class Assets {
private:
	std::string file_path;
	std::ifstream file;
	std::string word;

	Components recipe_player;
	Components recipe_bullet;
	Components recipe_missle;
	std::map<int, Components> recipe_enemy;

	std::map<size_t, sf::Font> fonts;
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Sprite> sprites;
	std::map<std::string, Border> borders;

	void loadEntities();
	void loadFonts();
	void loadGUI();

	void loadEntity();
	void loadTexture();
	void loadSprite();
	void loadBorders();
	void loadBorderRepeatable(std::string border_name, std::string sprite_name, std::string texture_name);
	sf::IntRect loadRect();
	sf::Texture makeRepeatable(const sf::Texture& original, sf::IntRect& rect);

public:
	enum {
		NONE,
		FONT_COURIER,
		FONT_MILITARY
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
	Border& getBorder(std::string name);
	sf::Texture& getTexture(std::string name);
	sf::Sprite& getSprite(std::string name);

};

#endif
