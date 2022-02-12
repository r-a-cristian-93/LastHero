#include <fstream>
#include <iostream>
#include "Common.h"
#include "Assets.h"
#include "Settings.h"

Assets::Assets() {
	loadGUI();
	loadEntities();
	loadFonts();
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

Components& Assets::getRecipeEnemy(std::string& recipe_name) {
	return recipe_enemy[recipe_name];
}

Components& Assets::getRecipeEnemyRand() {
	std::map<std::string, Components>::iterator it;
	int k = recipe_enemy.size() - rand() % recipe_enemy.size();

	for (it = recipe_enemy.begin(); it != recipe_enemy.end(); it++) {
		k--;
		if (k <= 0) {
			return it->second;
		}
	}
}

sf::Font& Assets::getFont(size_t name) {
	return fonts[name];
}

Border& Assets::getBorder(std::string name) {
	return borders[name];
}

sf::Texture& Assets::getTexture(std::string name) {
	return textures[name];
}

sf::Sprite& Assets::getSprite(std::string name) {
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
		else if (word == "name") file >> data_ent.name;
		else if (word == "radius") file >> data_ent.radius;
		else if (word == "velocity") file >> data_ent.velocity;
		else if (word == "fill") {
			int r, g, b;
			file >> r >> g >> b;
			data_ent.fill = sf::Color(r,g,b);
		}
		else if (word == "outline") {
			int r, g, b;
			file >> r >> g >> b >> data_ent.out_thk;
			data_ent.outline = sf::Color(r,g,b);
		}
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
		else if (word == "animation") {
			file >> word;
			loadAnimationSet(word, data_ent.animation_set);
		}
	}

	switch (data_ent.type) {
		case Entity::TAG_PLAYER: {
			sf::CircleShape shape(data_ent.radius, data_ent.vertices);
			shape.setOrigin(data_ent.radius, data_ent.radius);
			shape.setFillColor(data_ent.fill);
			shape.setOutlineColor(data_ent.outline);
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
			recipe_player.add<CAnimation>(new CAnimation(data_ent.animation_set));
		}
		break;
		case Entity::TAG_BULLET: {
			sf::CircleShape shape(data_ent.radius, data_ent.vertices);
			shape.setOrigin(data_ent.radius, data_ent.radius);
			shape.setFillColor(data_ent.fill);

			recipe_bullet.add<CTransform>(new CTransform(data_ent.velocity));
			recipe_bullet.add<CShape>(new CShape(shape));
			recipe_bullet.add<CCollision>(new CCollision(data_ent.radius));
			recipe_bullet.add<CLifespan>(new CLifespan(data_ent.lifespan));
		}
		break;
		case Entity::TAG_MISSLE: {
			sf::CircleShape shape(data_ent.radius, data_ent.vertices);
			shape.setOrigin(data_ent.radius, data_ent.radius);
			shape.setFillColor(data_ent.fill);

			recipe_missle.add<CTransform>(new CTransform(data_ent.velocity));
			recipe_missle.add<CShape>(new CShape(shape));
			recipe_missle.add<CCollision>(new CCollision(data_ent.radius));
			recipe_missle.add<CTarget>(new CTarget());
		}
		break;
		case Entity::TAG_ENEMY: {
			sf::CircleShape shape(data_ent.radius, data_ent.vertices);
			shape.setOrigin(data_ent.radius, data_ent.radius);
			shape.setFillColor(data_ent.fill);
			shape.setOutlineColor(data_ent.outline);
			shape.setOutlineThickness(data_ent.out_thk);

			CStats stats;
			stats.experience = data_ent.experience;
			stats.level = data_ent.level;

			for (int i=0; i<CStats::COUNT; i++) {
				stats.base[i] = data_ent.stats_base[i];
				stats.per_level[i] = data_ent.stats_per_level[i];
			}

			recipe_enemy[data_ent.name].add<CTransform>(new CTransform(data_ent.velocity));
			recipe_enemy[data_ent.name].add<CShape>(new CShape(shape));
			recipe_enemy[data_ent.name].add<CCollision>(new CCollision(data_ent.radius));
			recipe_enemy[data_ent.name].add<CScore>(new CScore(data_ent.vertices));
			recipe_enemy[data_ent.name].add<CStats>(new CStats(stats));
		}
		break;
	}
}

void Assets::loadAnimationSet(std::string path, AnimationSet& anim_set) {
	file_two.open(path);

	while (file_two >> word) {
		if (word == "_ANIM") {
			loadAnimation(anim_set);
		}
	}
}

void Assets::loadAnimation(AnimationSet& anim_set) {
	size_t state(Entity::STATE_IDLE);
	size_t play(0), frames(0);
	std::string texture_name("");
	std::vector<sf::IntRect> rects;
	std::vector<size_t> frame_time;
	size_t flip_x(0), flip_y(0);
	sf::Vector2f origin(0,0), frame_size(0,0), frame_offset(0,0);

	while (file_two >> word) {
		if (word == "_END") break;
		else if (word == "texture") {
			file_two >> texture_name;
		}
		else if (word == "type") {
			file_two >> word;
			if (word == "stand") state = Entity::STATE_IDLE;
			else if (word == "move") state = Entity::STATE_RUN;
			else if (word == "die") state = Entity::STATE_DIE;
		}
		else if (word == "play") {
			file_two >> word;
			if (word == "once") play = Animation::PLAY_ONCE;
			else if (word == "loop") play = Animation::PLAY_LOOP;
			else if (word == "swing") play = Animation::PLAY_SWING;
		}
		else if (word == "frames") {
			file_two >> frames;
		}
		else if (word == "origin") {
			file_two >> origin.x >> origin.y;
		}
		else if (word == "frame_size") {
			file_two >> frame_size.x >> frame_size.y;
		}
		else if (word == "frame_offset") {
			file_two >> frame_offset.x >> frame_offset.y;
		}
		else if (word == "flip") {
			file_two >> flip_x >> flip_y;
		}
		else if (word == "frame_time") {
			for (int i=0; i<frames; i++) {
				size_t f_t;
				file_two >> f_t;
				frame_time.push_back(f_t);
			}
		}
	}

	for (int dir=0; dir<8; dir++) {
		std::vector<sf::Sprite> sprites;

		for (int f=0; f<frames; f++) {
			sf::IntRect rect(origin.x+frame_size.x*f, origin.y + frame_size.y*(dir), frame_size.x, frame_size.y);
			sprites.push_back(sf::Sprite(textures[texture_name], rect));
			sprites.back().setOrigin(frame_offset.x, frame_offset.y);
		}

		anim_set.animations[state][dir+1] = Animation(sprites, frame_time, play);
	}
}

void Assets::flipRectX(sf::IntRect& rect) {
	rect.left += rect.width;
	rect.width = -rect.width;
}

void Assets::flipRectY(sf::IntRect& rect) {
	rect.top += rect.height;
	rect.height = -rect.height;
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
	file_path = "res/gui.cfg";
	file.open(file_path);

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
	std::string texture_name;
	std::string border_name;

	while (file >> word) {
		if (word == "_END") break;
		else if (word == "texture") {
			file >> texture_name;
		}
		else if (word == "name") {
			file >> border_name;
		}
		else if (word == "top_left") {
			borders[border_name].setSprite(Border::TOP_LEFT, sf::Sprite(textures[texture_name], loadRect(file)));
		}
		else if (word == "top_center") {
			loadBorderRepeatable(border_name, Border::TOP_CENTER, texture_name);
		}
		else if (word == "top_right") {
			borders[border_name].setSprite(Border::TOP_RIGHT, sf::Sprite(textures[texture_name], loadRect(file)));
		}
		else if (word == "bottom_left") {
			borders[border_name].setSprite(Border::BOTTOM_LEFT, sf::Sprite(textures[texture_name], loadRect(file)));
		}
		else if (word == "bottom_center") {
			loadBorderRepeatable(border_name, Border::BOTTOM_CENTER, texture_name);
		}
		else if (word == "bottom_right") {
			borders[border_name].setSprite(Border::BOTTOM_RIGHT, sf::Sprite(textures[texture_name], loadRect(file)));
		}
		else if (word == "middle_left") {
			loadBorderRepeatable(border_name, Border::MIDDLE_LEFT, texture_name);
		}
		else if (word == "middle_right") {
			loadBorderRepeatable(border_name, Border::MIDDLE_RIGHT, texture_name);
		}
		else {
			std::cout << "In file: " << file_path << " unknown key: " << word << std::endl;
		}
	}
}

sf::IntRect Assets::loadRect(std::ifstream& f) {
	sf::IntRect rect;
	f >> rect.left;
	f >> rect.top;
	f >> rect.width;
	f >> rect.height;
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

void Assets::loadBorderRepeatable(std::string border_name, size_t sprite_name, std::string texture_name) {
	sf::IntRect rect = loadRect(file);
	sf::Texture tex = makeRepeatable(textures[texture_name], rect);
	borders[border_name].setTexture(sprite_name, tex);
	borders[border_name].setSprite(sprite_name, sf::Sprite(borders[border_name].getTexture(sprite_name), rect));
}

void Assets::loadTexture() {
	std::string name("");
	unsigned int w, h;
	std::string* path;
	sf::Color* color(nullptr);

	while (file >> word) {
		if (word == "_END") break;
		else if (word == "name") {
			file >> name;
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
		else {
			std::cout << "In file: " << file_path << " unknown key: " << word << std::endl;
		}
	}

	if (color) {
		sf::Image img;
		sf::Texture tex;
		img.create(w, h, *color);
		tex.loadFromImage(img);
		tex.setRepeated(true);
		textures[name] = tex;
		delete color;
	}
	else if (path) {
		if (!textures[name].loadFromFile(*path)) {
			std::cout << "Can't load texture " << path << std::endl;
		}
	}
}

void Assets::loadSprite() {
	std::string texture_name("");
	std::string sprite_name("");
	sf::Vector2f origin(0, 0);

	while (file >> word) {
		if (word == "_END") break;
		else if (word == "texture") {
			file >> texture_name;
		}
		else if (word == "name") {
			file >> sprite_name;
		}
		else if (word == "rectangle") {
			sprites[sprite_name] = sf::Sprite(textures[texture_name], loadRect(file));
		}
		else if (word == "origin") {
			file >> origin.x >> origin.y;
			sprites[sprite_name].setOrigin(origin);
		}
		else {
			std::cout << "In file: " << file_path << " unknown key: " << word << std::endl;
		}
	}
}
