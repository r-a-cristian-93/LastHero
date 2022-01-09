#include <fstream>
#include <iostream>
#include "Common.h"
#include "Assets.h"
#include "Settings.h"

Assets::Assets() {
	loadEntities();
}

Components& Assets::getRecipePlayer() {
	return recipe_player;
}

Components& Assets::getRecipeBullet() {
	return recipe_bullet;
}

Components& Assets::getRecipeMissle() {
	return recipe_missle;
}

Components& Assets::getRecipeEnemy(size_t recipe_id) {
	return recipe_enemy[recipe_id];
}

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
			if (word == "player") data_ent.type = Entity::TAG_PLAYER;
			else if (word == "bullet") data_ent.type = Entity::TAG_BULLET;
			else if (word == "missle") data_ent.type = Entity::TAG_MISSLE;
			else if (word == "enemy") data_ent.type = Entity::TAG_ENEMY;
			else data_ent.type = Entity::NONE;
		}
		else if (word == "id") file >> data_ent.id;
		else if (word == "radius") file >> data_ent.radius;
		else if (word == "velocity") file >> data_ent.velocity;
		else if (word == "fill_r") file >> data_ent.fill_r;
		else if (word == "fill_g") file >> data_ent.fill_g;
		else if (word == "fill_b") file >> data_ent.fill_b;
		else if (word == "out_r") file >> data_ent.out_r;
		else if (word == "out_g") file >> data_ent.out_g;
		else if (word == "out_b") file >> data_ent.out_b;
		else if (word == "out_thk") file >> data_ent.out_thk;
		else if (word == "vertices") file >> data_ent.vertices;
		else if (word == "lifespan") file >> data_ent.lifespan;
	}

	switch (data_ent.type) {
		case Entity::TAG_PLAYER: {
			sf::CircleShape shape(data_ent.radius, data_ent.vertices);
			shape.setOrigin(data_ent.radius, data_ent.radius);
			shape.setFillColor(sf::Color(data_ent.fill_r, data_ent.fill_g, data_ent.fill_b));
			shape.setOutlineColor(sf::Color(data_ent.out_r, data_ent.out_g, data_ent.out_b));
			shape.setOutlineThickness(data_ent.out_thk);

			recipe_player.add<CTransform>(new CTransform(data_ent.velocity));
			recipe_player.add<CShape>(new CShape(shape));
			recipe_player.add<CCollision>(new CCollision(data_ent.radius));
			recipe_player.add<CInput>(new CInput());
		}
		break;
		case Entity::TAG_BULLET: {
			sf::CircleShape shape(data_ent.radius, data_ent.vertices);
			shape.setOrigin(data_ent.radius, data_ent.radius);
			shape.setFillColor(sf::Color(data_ent.fill_r, data_ent.fill_g, data_ent.fill_b));

			recipe_bullet.add<CTransform>(new CTransform(data_ent.velocity));
			recipe_bullet.add<CShape>(new CShape(shape));
			recipe_bullet.add<CCollision>(new CCollision(data_ent.radius));
			recipe_bullet.add<CLifespan>(new CLifespan(data_ent.lifespan));
		}
		break;
		case Entity::TAG_MISSLE: {
			sf::CircleShape shape(data_ent.radius, data_ent.vertices);
			shape.setOrigin(data_ent.radius, data_ent.radius);
			shape.setFillColor(sf::Color(data_ent.fill_r, data_ent.fill_g, data_ent.fill_b));

			recipe_missle.add<CTransform>(new CTransform(data_ent.velocity));
			recipe_missle.add<CShape>(new CShape(shape));
			recipe_missle.add<CCollision>(new CCollision(data_ent.radius));
			recipe_missle.add<CTarget>(new CTarget());
		}
		break;
		case Entity::TAG_ENEMY: {
			sf::CircleShape shape(data_ent.radius, data_ent.vertices);
			shape.setOrigin(data_ent.radius, data_ent.radius);
			shape.setFillColor(sf::Color(data_ent.fill_r, data_ent.fill_g, data_ent.fill_b));
			shape.setOutlineColor(sf::Color(data_ent.out_r, data_ent.out_g, data_ent.out_b));
			shape.setOutlineThickness(data_ent.out_thk);

			recipe_enemy[data_ent.id].add<CTransform>(new CTransform(data_ent.velocity));
			recipe_enemy[data_ent.id].add<CShape>(new CShape(shape));
			recipe_enemy[data_ent.id].add<CCollision>(new CCollision(data_ent.radius));
			recipe_enemy[data_ent.id].add<CScore>(new CScore(data_ent.vertices));
		}
		break;
	}
}
