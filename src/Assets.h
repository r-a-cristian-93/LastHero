#ifndef ASSETS
#define ASSETS

#include <vector>
#include <string>
#include <fstream>
#include "Entity.h"

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
	EntityDataset data_ent;

	Components recipe_player;
	Components recipe_bullet;
	Components recipe_missle;
	std::map<int, Components> recipe_enemy;

	std::map<size_t, sf::Font> fonts;

	void loadEntities();
	void loadEntity();
	void loadFonts();

	template<class T>
	void add(Components& c, T* t) {
		std::get<T*>(c) = t;
	}

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

};

#endif
