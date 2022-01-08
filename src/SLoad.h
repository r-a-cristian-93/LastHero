#ifndef S_LOAD
#define S_LOAD

#include <vector>
#include <string>
#include <fstream>
#include "Entity.h"
#include "EntityManager.h"

namespace SLoad {

enum {
	ENT_PLAYER,
	ENT_BULLET,
	ENT_MISSLE,
	ENT_ENEMY
};

struct Dataset {
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

} dataset;

	std::ifstream file;
	std::string word;

	Components recipe_player;
	Components recipe_bullet;
	Components recipe_missle;
	std::map<int, Components> recipe_enemies;

	void loadSettings(std::string file_name);
	void loadEntities(std::string file_name, EntityManager& em);

	void loadEntities();
	void loadEntity();

	template<class T>
	void add(Components& c, T* t) {
		std::get<T*>(c) = t;
	}

	template<class T>
	T* get(Components& c) {
		return std::get<T*>(c);
	}
}

#endif
