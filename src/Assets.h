#ifndef ASSETS
#define ASSETS

#include <vector>
#include <string>
#include <fstream>
#include "Entity.h"

struct EntityDataset {
	size_t type;
	int id;
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

	void loadEntities();
	void loadEntity();

	template<class T>
	void add(Components& c, T* t) {
		std::get<T*>(c) = t;
	}

public:
	Assets();

	template<class T>
	const T* get(Components& c) {
		return std::get<T*>(c);
	}

	Components& getRecipePlayer();
};

#endif
