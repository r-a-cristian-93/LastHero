#include <fstream>
#include <iostream>
#include "Common.h"
#include "Assets.h"
#include "Settings.h"

Assets::Assets() {
	loadEntities();
	loadFonts();
	loadTextures();
	loadSprites();
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

sf::Font& Assets::getFont(size_t name) {
	return fonts[name];
}

Border& Assets::getBorder(size_t name) {
	return borders[name];
}

void Assets::loadEntities() {
	file.open("res/entities.cfg");

	while (file >> word) {
		if (word == "_ENT") {
			loadEntity();
		}
	}

	file.close();
}

void Assets::loadEntity() {
	EntityDataset data_ent;

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

void Assets::loadFonts() {
	if (!fonts[FONT_COURIER].loadFromFile("res/courier.ttf")) {
		std::cout << "Can't load font COURIER\n";
	}

	if (!fonts[FONT_MILITARY].loadFromFile("res/military.ttf")) {
		std::cout << "Can't load font MILITARY\n";
	}
}

void Assets::loadTextures() {
	if (!textures[TEX_GUI].loadFromFile("res/images/gui.png")) {
		std::cout << "Can't load texture res/images/gui.png";
	}
}

void Assets::loadSprites() {
	file.open("res/gui.cfg");

	while (file >> word) {
		if (word == "_BORDERS") {
			loadBorders();
		}
	}

	std::cout << "Loaded borders: " << borders.size() << std::endl;
	int w = borders[BORDER_SLICK].getSprite(Border::TOP_LEFT).getTextureRect().width;
	std::cout << "W " << w << std::endl;

	file.close();
}

void Assets::loadBorders() {
	size_t id = 0;

	while (file >> word) {
		if (word == "_END") break;
		else if (word == "name") {
			file >> word;
			if (word == "slick") id = BORDER_SLICK;
			else if (word == "thick") id = BORDER_THICK;
			///borders[id] = new Border();
		}
		else if (word == "top_left") {
			borders[id].setSprite(Border::TOP_LEFT, sf::Sprite(textures[TEX_GUI], loadRect()));
		}
		else if (word == "top_center") {
			borders[id].setSprite(Border::TOP_CENTER, sf::Sprite(textures[TEX_GUI], loadRect()));
		}
		else if (word == "top_right") {
			borders[id].setSprite(Border::TOP_RIGHT, sf::Sprite(textures[TEX_GUI], loadRect()));
		}
		else if (word == "bottom_left") {
			borders[id].setSprite(Border::BOTTOM_LEFT, sf::Sprite(textures[TEX_GUI], loadRect()));
		}
		else if (word == "bottom_center") {
			borders[id].setSprite(Border::BOTTOM_CENTER, sf::Sprite(textures[TEX_GUI], loadRect()));
		}
		else if (word == "bottom_right") {
			borders[id].setSprite(Border::BOTTOM_RIGHT, sf::Sprite(textures[TEX_GUI], loadRect()));
		}
		else if (word == "middle_left") {
			borders[id].setSprite(Border::MIDDLE_LEFT, sf::Sprite(textures[TEX_GUI], loadRect()));
		}
		else if (word == "middle_right") {
			borders[id].setSprite(Border::MIDDLE_RIGHT, sf::Sprite(textures[TEX_GUI], loadRect()));
		}
	}
}

void Assets::loadRect(sf::IntRect& rect) {
	file >> rect.left;
	file >> rect.top;
	file >> rect.width;
	file >> rect.height;
}

sf::IntRect Assets::loadRect() {
	sf::IntRect rect;
	file >> rect.left;
	file >> rect.top;
	file >> rect.width;
	file >> rect.height;
	return rect;
}
