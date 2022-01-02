#include <fstream>
#include <iostream>
#include "Common.h"
#include "SLoad.h"
#include "Settings.h"


void SLoad::loadSettings(std::string file_name) {
	std::ifstream file(file_name);
	
	//input data
	std::string word;
	std::string font_file_path;
	int font_size;
	sf::Color font_color;
	sf::Font font;
	
	while(file >> word) {
		if (word == "Window") {
			file >> Conf->w_width >> Conf->w_height;
		}
		if (word == "Font") {
			file >> font_file_path >> Conf->font_size >> Conf->font_color.r >> Conf->font_color.g >> Conf->font_color.b;
			
			if (!Conf->font.loadFromFile(font_file_path)) {
				std::cerr << "Could not load font!\n";
				exit(-1);
			}
		}
	}
	
	file.close();
}

void SLoad::loadEntities(std::string file_name, EntityManager& em) {
	std::ifstream file(file_name);
	
	//input data
	std::string word;
	
	PlayerConfig p_conf;
	EnemyConfig e_conf;
	BulletConfig b_conf;
	
	while(file >> word) {		
		if (word == "Player") {
			file >> p_conf.shape_radius;
			file >> p_conf.collision_radius;
			file >> p_conf.velocity;
			file >> p_conf.fill_r;
			file >> p_conf.fill_g;
			file >> p_conf.fill_b;
			file >> p_conf.out_r;
			file >> p_conf.out_g;
			file >> p_conf.out_b;
			file >> p_conf.out_thk;
			file >> p_conf.vertices;
		}
		if (word == "Enemy") {
			file >> e_conf.shape_radius;
			file >> e_conf.collision_radius;
			file >> e_conf.velocity_min;
			file >> e_conf.velocity_max;
			file >> e_conf.out_r;
			file >> e_conf.out_g;
			file >> e_conf.out_b;
			file >> e_conf.out_thk;
			file >> e_conf.vertices_min;
			file >> e_conf.vertices_max;
		}
		if (word == "Bullet") {
			file >> b_conf.shape_radius;
			file >> b_conf.collision_radius;
			file >> b_conf.velocity;
			file >> p_conf.fill_r;
			file >> p_conf.fill_g;
			file >> p_conf.fill_b;
			file >> b_conf.out_r;
			file >> b_conf.out_g;
			file >> b_conf.out_b;
			file >> b_conf.out_thk;
			file >> b_conf.vertices;
			file >> b_conf.lifespan;
		}
	}

	file.close();
}
