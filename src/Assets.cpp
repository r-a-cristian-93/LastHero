#include <fstream>
#include <iostream>
#include "Common.h"
#include "Assets.h"
#include "Settings.h"

Assets::Assets() {
	loadEntities();
	loadFonts();
	loadGUI();
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

sf::Texture& Assets::getTexture(size_t name) {
	return textures[name];
}

sf::Sprite& Assets::getSprite(size_t name) {
	return sprites[name];
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
		else if (word == "experience") file >> data_ent.experience;
		else if (word == "level") file >> data_ent.level;
		else if (word == "stats_base") {
			for (int i=0; i<CStats::COUNT; i++) {
				file >> data_ent.stats_base[i];
			}
		}
		else if (word == "stats_per_level") {
			for (int i=0; i<CStats::COUNT; i++) {
				file >> data_ent.stats_per_level[i];
			}
		}
	}

	switch (data_ent.type) {
		case Entity::TAG_PLAYER: {
			sf::CircleShape shape(data_ent.radius, data_ent.vertices);
			shape.setOrigin(data_ent.radius, data_ent.radius);
			shape.setFillColor(sf::Color(data_ent.fill_r, data_ent.fill_g, data_ent.fill_b));
			shape.setOutlineColor(sf::Color(data_ent.out_r, data_ent.out_g, data_ent.out_b));
			shape.setOutlineThickness(data_ent.out_thk);

			CStats stats;
			stats.experience = data_ent.experience;
			stats.level = data_ent.level;

			for (int i=0; i<CStats::COUNT; i++) {
				stats.base[i] = data_ent.stats_base[i];
				stats.per_level[i] = data_ent.stats_per_level[i];
			}

			recipe_player.add<CTransform>(new CTransform(data_ent.velocity));
			recipe_player.add<CShape>(new CShape(shape));
			recipe_player.add<CCollision>(new CCollision(data_ent.radius));
			recipe_player.add<CInput>(new CInput());
			recipe_player.add<CStats>(new CStats(stats));
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

			CStats stats;
			stats.experience = data_ent.experience;
			stats.level = data_ent.level;

			for (int i=0; i<CStats::COUNT; i++) {
				stats.base[i] = data_ent.stats_base[i];
				stats.per_level[i] = data_ent.stats_per_level[i];
			}

			recipe_enemy[data_ent.id].add<CTransform>(new CTransform(data_ent.velocity));
			recipe_enemy[data_ent.id].add<CShape>(new CShape(shape));
			recipe_enemy[data_ent.id].add<CCollision>(new CCollision(data_ent.radius));
			recipe_enemy[data_ent.id].add<CScore>(new CScore(data_ent.vertices));
			recipe_enemy[data_ent.id].add<CStats>(new CStats(stats));
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

void Assets::loadGUI() {
	file.open("res/gui.cfg");

	while (file >> word) {
		if (word == "_TEXTURE") {
			loadTexture();
		}
		else if (word == "_BORDERS") {
			loadBorders();
		}
		else if (word == "_SPRITE") {
			loadSprite();
		}
	}

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
		}
		else if (word == "top_left") {
			borders[id].setSprite(Border::TOP_LEFT, sf::Sprite(textures[TEX_GUI], loadRect()));
		}
		else if (word == "top_center") {
			loadBorderRepeatable(id, Border::TEX_TOP_CENTER, Border::TOP_CENTER);
		}
		else if (word == "top_right") {
			borders[id].setSprite(Border::TOP_RIGHT, sf::Sprite(textures[TEX_GUI], loadRect()));
		}
		else if (word == "bottom_left") {
			borders[id].setSprite(Border::BOTTOM_LEFT, sf::Sprite(textures[TEX_GUI], loadRect()));
		}
		else if (word == "bottom_center") {
			loadBorderRepeatable(id, Border::TEX_BOTTOM_CENTER, Border::BOTTOM_CENTER);
		}
		else if (word == "bottom_right") {
			borders[id].setSprite(Border::BOTTOM_RIGHT, sf::Sprite(textures[TEX_GUI], loadRect()));
		}
		else if (word == "middle_left") {
			loadBorderRepeatable(id, Border::TEX_MIDDLE_LEFT, Border::MIDDLE_LEFT);
		}
		else if (word == "middle_right") {
			loadBorderRepeatable(id, Border::TEX_MIDDLE_RIGHT, Border::MIDDLE_RIGHT);
		}
	}
}

sf::IntRect Assets::loadRect() {
	sf::IntRect rect;
	file >> rect.left;
	file >> rect.top;
	file >> rect.width;
	file >> rect.height;
	return rect;
}

sf::Texture Assets::makeRepeatable(const sf::Texture& original, sf::IntRect& rect) {
	sf::Image img = original.copyToImage();
	sf::Texture tex;
	tex.loadFromImage(img, rect);
	tex.setRepeated(true);
	rect.left = 0;
	rect.top = 0;
	return tex;
}

void Assets::loadBorderRepeatable(size_t id, size_t texture_name, size_t sprite_name) {
	sf::IntRect rect = loadRect();
	sf::Texture tex = makeRepeatable(textures[TEX_GUI], rect);
	borders[id].setTexture(texture_name, tex);
	borders[id].setSprite(sprite_name, sf::Sprite(borders[id].getTexture(texture_name), rect));
}

void Assets::loadTexture() {
	size_t id = 0;
	unsigned int w, h;
	std::string* path;
	sf::Color* color(nullptr);

	while (file >> word) {
		if (word == "_END") break;
		else if (word == "name") {
			file >> word;
			if (word == "gui") id = Assets::TEX_GUI;
			else if (word == "dark_green") id = Assets::TEX_FILL_DARK_GREEN;
		}
		else if (word == "path") {
			path = new std::string();
			file >> *path;
		}
		else if (word == "size") file >> w >> h;
		else if (word == "color") {
			size_t r, g ,b;
			file >> r >> g >> b;
			color = new sf::Color(r, g, b);
		}
	}

	if (color) {
		sf::Image img;
		sf::Texture tex;
		img.create(w, h, *color);
		tex.loadFromImage(img);
		tex.setRepeated(true);
		textures[id] = tex;
		delete color;
	}
	else if (path) {
		if (!textures[id].loadFromFile(*path)) {
			std::cout << "Can't load texture " << path << std::endl;
		}
	}
}

void Assets::loadSprite() {
	size_t id = 0;

	while (file >> word) {
		if (word == "_END") break;
		else if (word == "name") {
			file >> word;
			if (word == "icon_skull") id = SPRITE_ICON_SKULL;
			if (word == "icon_hart") id = SPRITE_ICON_HART;
			if (word == "icon_helmet") id = SPRITE_ICON_HELMET;
		}
		else if (word == "rectangle") {
			sprites[id] = sf::Sprite(textures[TEX_GUI], loadRect());
		}
	}
}
