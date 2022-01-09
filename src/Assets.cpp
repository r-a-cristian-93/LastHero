#include <fstream>
#include <iostream>
#include "Common.h"
#include "Assets.h"
#include "Settings.h"

Assets::Assets() {}

void Assets::loadEntities() {
	file.open("res/entities.cfg");

	while (file >> word) {
		if (word == "_ENT") {
			loadEntity();
		}
	}
}

void Assets::loadEntity() {
	while (file >> word) {
		if (word == "_END") break;
		else if (word == "type") {
			file >> word;
			if (word == "player") dataset.type = ENT_PLAYER;
			else if (word == "bullet") dataset.type = ENT_BULLET;
			else if (word == "missle") dataset.type = ENT_MISSLE;
			else if (word == "enemy") dataset.type = ENT_ENEMY;
			else dataset.type = NONE;
		}
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
