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

void SLoad::loadEntities() {
	file.open("res/entities.cfg");

	while (file >> word) {
		if (word == "_ENT") {
			loadEntity();
		}
	}
}

void SLoad::loadEntity() {
	while (file >> word) {
		if (word == "_END") break;
		else if (word == "type") file >> dataset.type;
		else if (word == "radius") file >> dataset.radius;
		else if (word == "velocity") file >> dataset.velocity;
		else if (word == "fill_r") file >> dataset.fill_r;
		else if (word == "fill_g") file >> dataset.fill_g;
		else if (word == "fill_b") file >> dataset.fill_b;
		else if (word == "out_r") file >> dataset.out_r;
		else if (word == "out_g") file >> dataset.out_g;
		else if (word == "out_b") file >> dataset.out_b;
		else if (word == "out_thk") file >> dataset.out_thk;
		else if (word == "vertices") file >> dataset.vertices;
		else if (word == "lifespan") file >> dataset.lifespan;
	}

	switch (dataset.type) {
		case ENT_PLAYER:
			std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(dataset.radius, dataset.vertices);
			shape->setOrigin(dataset.radius, dataset.radius);
			shape->setFillColor(sf::Color(dataset.fill_r, dataset.fill_g, dataset.fill_b));
			shape->setOutlineColor(sf::Color(dataset.out_r, dataset.out_g, dataset.out_b));
			shape->setOutlineThickness(dataset.out_thk);

			recipe_player.add<CTransform>(new CTransform(dataset.velocity));
			recipe_player.add<CShape>(new CShape(shape));
			recipe_player.add<CCollision>(new CCollision(dataset.radius));
			recipe_player.add<CInput>(new CInput());
		break;
	}
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
			file >> b_conf.fill_r;
			file >> b_conf.fill_g;
			file >> b_conf.fill_b;
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
