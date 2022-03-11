#include <fstream>
#include <iostream>
#include "Common.h"
#include "Assets.h"
#include "Settings.h"

Assets::Assets() {
	loadGUI();
	loadEntities();
	loadFonts();
	loadShaders();
}

Components& Assets::getRecipe(size_t tag, std::string& recipe_name) {
	return recipe[tag][recipe_name];
}

Components& Assets::getRecipeRand(size_t tag) {
	std::map<std::string, Components>::iterator it;
	int k = recipe[tag].size() - rand() % recipe[tag].size();

	for (it = recipe[tag].begin(); it != recipe[tag].end(); it++) {
		k--;
		if (k <= 0) {
			return it->second;
		}
	}
	return recipe[tag][""];
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
			else if (word == "projectile") data_ent.type = Entity::TAG_PROJECTILE;
			else if (word == "missle") data_ent.type = Entity::TAG_MISSLE;
			else if (word == "enemy") data_ent.type = Entity::TAG_ENEMY;
			else if (word == "sfx") data_ent.type = Entity::TAG_SFX;
			else data_ent.type = Entity::NONE;
		}
		else if (word == "name") file >> data_ent.name;
		else if (word == "velocity") file >> data_ent.velocity;
		else if (word == "fill") {
			int r, g, b, a;
			file >> r >> g >> b >> a;
			data_ent.fill = sf::Color(r,g,b, a);
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
			data_ent.stats_base = new int[CStats::COUNT];
			for (int i=0; i<CStats::COUNT; i++) {
				file >> data_ent.stats_base[i];
			}
		}
		else if (word == "stats_per_level") {
			data_ent.stats_per_level = new int[CStats::COUNT];
			for (int i=0; i<CStats::COUNT; i++) {
				file >> data_ent.stats_per_level[i];
			}
		}
		else if (word == "animation") {
			file >> word;
			loadAnimationSet(word, data_ent.animation_set);
		}
		else if (word == "color_mod") {
			int r, g, b, a;
			file >> r >> g >> b >> a;
			data_ent.color_mod = sf::Color(r,g,b,a);
		}
		else if (word == "weapon_primary") {
			file >> word;

			if (word == "player") data_ent.p_tag = Entity::TAG_PLAYER;
			else if (word == "projectile") data_ent.p_tag = Entity::TAG_PROJECTILE;
			else if (word == "missle") data_ent.p_tag = Entity::TAG_MISSLE;
			else if (word == "enemy") data_ent.p_tag = Entity::TAG_ENEMY;
			else if (word == "sfx") data_ent.p_tag = Entity::TAG_SFX;

			file >> data_ent.weapon_primary >> data_ent.primary_cooldown;
		}
		else if (word == "weapon_secondary") {
			file >> word;

			if (word == "player") data_ent.s_tag = Entity::TAG_PLAYER;
			else if (word == "projectile") data_ent.s_tag = Entity::TAG_PROJECTILE;
			else if (word == "missle") data_ent.s_tag = Entity::TAG_MISSLE;
			else if (word == "enemy") data_ent.s_tag = Entity::TAG_ENEMY;
			else if (word == "sfx") data_ent.s_tag = Entity::TAG_SFX;

			file >> data_ent.weapon_secondary >> data_ent.secondary_cooldown;
		}
		else if (word == "projectile_spawn") {
			for (int i=1; i<=8; i++) {
				float dx, dy;
				file >> dx >> dy;
				data_ent.projectile_spawn[i].x = dx;
				data_ent.projectile_spawn[i].y = dy;
			}
		}
		else if (word == "hitbox") {
			HitBox hb;
			file >> hb.radius;

			for (int i=1; i<=8; i++) {
				float dx, dy;
				file >> dx >> dy;
				hb.offset[i].x = dx;
				hb.offset[i].y = dy;
			}
			data_ent.hitbox.push_back(hb);
		}
		else if (word == "prio") {
			file >> data_ent.prio;
		}
	}

	switch (data_ent.type) {
		case Entity::TAG_PLAYER: {
			sf::CircleShape shape;
			shape.setFillColor(data_ent.fill);
			shape.setOutlineColor(data_ent.outline);
			shape.setOutlineThickness(data_ent.out_thk);

			CStats stats;
			stats.experience = data_ent.experience;
			stats.level = data_ent.level;

			for (int i=0; i<CStats::COUNT; i++) {
				stats.base[i] = data_ent.stats_base[i];
				stats.per_level[i] = data_ent.stats_per_level[i];
				stats.initial[i] = stats.base[i] + stats.per_level[i] * stats.level;
				stats.effective[i] = stats.initial[i];
			}

			CWeapon weapon(data_ent.weapon_primary, data_ent.weapon_secondary);
			weapon.p_tag = data_ent.p_tag;
			weapon.s_tag = data_ent.s_tag;
			weapon.projectile_spawn = data_ent.projectile_spawn;
			weapon.p_cooldown = data_ent.primary_cooldown;
			weapon.s_cooldown = data_ent.secondary_cooldown;

			CCollision collision;
			if (!data_ent.hitbox.empty()) {
				collision.hitbox = data_ent.hitbox;
			}

			recipe[data_ent.type][data_ent.name].add<CTransform>(new CTransform(data_ent.velocity));
			recipe[data_ent.type][data_ent.name].add<CShape>(new CShape(shape));
			recipe[data_ent.type][data_ent.name].add<CCollision>(new CCollision(collision));
			recipe[data_ent.type][data_ent.name].add<CInput>(new CInput());
			recipe[data_ent.type][data_ent.name].add<CStats>(new CStats(stats));
			recipe[data_ent.type][data_ent.name].add<CAnimation>(new CAnimation(data_ent.animation_set));
			recipe[data_ent.type][data_ent.name].get<CAnimation>()->anim_set.setColorMod(data_ent.color_mod);
			recipe[data_ent.type][data_ent.name].get<CAnimation>()->prio = data_ent.prio;
			recipe[data_ent.type][data_ent.name].add<CWeapon>(new CWeapon(weapon));
		}
		break;
		case Entity::TAG_PROJECTILE: {
			sf::CircleShape shape;
			shape.setFillColor(data_ent.fill);

			// add CStats if exists
			if (data_ent.stats_base != nullptr) {
				if (data_ent.stats_per_level == nullptr) {
					data_ent.stats_per_level = new int[CStats::COUNT];
				}

				CStats stats;
				stats.experience = data_ent.experience;
				stats.level = data_ent.level;

				for (int i=0; i<CStats::COUNT; i++) {
					stats.base[i] = data_ent.stats_base[i];
					stats.per_level[i] = data_ent.stats_per_level[i];
					stats.initial[i] = stats.base[i] + stats.per_level[i] * stats.level;
					stats.effective[i] = stats.initial[i];
				}
				recipe[data_ent.type][data_ent.name].add<CStats>(new CStats(stats));
			}


			CCollision collision;
			if (!data_ent.hitbox.empty()) {
				collision.hitbox = data_ent.hitbox;
			}

			recipe[data_ent.type][data_ent.name].add<CTransform>(new CTransform(data_ent.velocity));
			recipe[data_ent.type][data_ent.name].add<CShape>(new CShape(shape));
			recipe[data_ent.type][data_ent.name].add<CCollision>(new CCollision());
			recipe[data_ent.type][data_ent.name].add<CLifespan>(new CLifespan(data_ent.lifespan));
			recipe[data_ent.type][data_ent.name].add<CCollision>(new CCollision(collision));


			if (data_ent.animation_set.animations.size() > 0) {
				recipe[data_ent.type][data_ent.name].add<CAnimation>(new CAnimation(data_ent.animation_set));
				recipe[data_ent.type][data_ent.name].get<CAnimation>()->anim_set.setColorMod(data_ent.color_mod);
				recipe[data_ent.type][data_ent.name].get<CAnimation>()->prio = data_ent.prio;
			}
		}
		break;
		case Entity::TAG_MISSLE: {
			sf::CircleShape shape;
			shape.setFillColor(data_ent.fill);

			recipe[data_ent.type][data_ent.name].add<CTransform>(new CTransform(data_ent.velocity));
			recipe[data_ent.type][data_ent.name].add<CShape>(new CShape(shape));
			recipe[data_ent.type][data_ent.name].add<CCollision>(new CCollision());
			recipe[data_ent.type][data_ent.name].add<CTarget>(new CTarget());
			if (data_ent.animation_set.animations.size() > 0) {
				recipe[data_ent.type][data_ent.name].add<CAnimation>(new CAnimation(data_ent.animation_set));
				recipe[data_ent.type][data_ent.name].get<CAnimation>()->anim_set.setColorMod(data_ent.color_mod);
				recipe[data_ent.type][data_ent.name].get<CAnimation>()->prio = data_ent.prio;
			}
		}
		break;
		case Entity::TAG_ENEMY: {
			sf::CircleShape shape;
			shape.setFillColor(data_ent.fill);
			shape.setOutlineColor(data_ent.outline);
			shape.setOutlineThickness(data_ent.out_thk);

			// add CStats if exists
			if (data_ent.stats_base != nullptr) {
				if (data_ent.stats_per_level == nullptr) {
					data_ent.stats_per_level = new int[CStats::COUNT];
				}

				CStats stats;
				stats.experience = data_ent.experience;
				stats.level = data_ent.level;

				for (int i=0; i<CStats::COUNT; i++) {
					stats.base[i] = data_ent.stats_base[i];
					stats.per_level[i] = data_ent.stats_per_level[i];
					stats.initial[i] = stats.base[i] + stats.per_level[i] * stats.level;
					stats.effective[i] = stats.initial[i];
				}
				recipe[data_ent.type][data_ent.name].add<CStats>(new CStats(stats));
			}

			CCollision collision;
			if (!data_ent.hitbox.empty()) {
				collision.hitbox = data_ent.hitbox;
			}

			recipe[data_ent.type][data_ent.name].add<CTransform>(new CTransform(data_ent.velocity));
			recipe[data_ent.type][data_ent.name].add<CShape>(new CShape(shape));
			recipe[data_ent.type][data_ent.name].add<CCollision>(new CCollision(collision));
			recipe[data_ent.type][data_ent.name].add<CScore>(new CScore(data_ent.vertices));
			if (data_ent.animation_set.animations.size() > 0) {
				recipe[data_ent.type][data_ent.name].add<CAnimation>(new CAnimation(data_ent.animation_set));
				recipe[data_ent.type][data_ent.name].get<CAnimation>()->anim_set.setColorMod(data_ent.color_mod);
				recipe[data_ent.type][data_ent.name].get<CAnimation>()->prio = data_ent.prio;
			}
			if (!data_ent.weapon_primary.empty() && !data_ent.weapon_secondary.empty()) {
				CWeapon weapon(data_ent.weapon_primary, data_ent.weapon_secondary);
				weapon.p_tag = data_ent.p_tag;
				weapon.s_tag = data_ent.s_tag;
				weapon.projectile_spawn = data_ent.projectile_spawn;
				weapon.p_cooldown = data_ent.primary_cooldown;
				weapon.s_cooldown = data_ent.secondary_cooldown;
				recipe[data_ent.type][data_ent.name].add<CWeapon>(new CWeapon(weapon));
				recipe[data_ent.type][data_ent.name].add<CInput>(new CInput());
			}
		}
		break;
		case Entity::TAG_SFX: {
			sf::CircleShape shape;
			shape.setFillColor(data_ent.fill);
			shape.setOutlineColor(data_ent.outline);
			shape.setOutlineThickness(data_ent.out_thk);

			recipe[data_ent.type][data_ent.name].add<CTransform>(new CTransform(data_ent.velocity));
			recipe[data_ent.type][data_ent.name].add<CLifespan>(new CLifespan(data_ent.lifespan));

			if (data_ent.animation_set.animations.size() > 0) {
				recipe[data_ent.type][data_ent.name].add<CAnimation>(new CAnimation(data_ent.animation_set));
				recipe[data_ent.type][data_ent.name].get<CAnimation>()->anim_set.setColorMod(data_ent.color_mod);
				recipe[data_ent.type][data_ent.name].get<CAnimation>()->prio = data_ent.prio;
			}
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

	file_two.close();
}

void Assets::loadAnimation(AnimationSet& anim_set) {
	size_t state(Entity::STATE_IDLE);
	size_t play(0), frames(0);
	std::string texture_name("");
	std::vector<sf::IntRect> rects;
	std::vector<size_t> frame_time;
	size_t flip_x(0), flip_y(0);
	std::vector<size_t> directions = {1, 2, 3, 4, 5, 6, 7, 8};
	sf::Vector2f origin(0,0), frame_size(0,0), frame_offset(0,0);

	while (file_two >> word) {
		if (word == "_END") break;
		else if (word == "texture") {
			file_two >> texture_name;
		}
		else if (word == "type") {
			file_two >> word;
			if (word == "idle") state = Entity::STATE_IDLE;
			else if (word == "run") state = Entity::STATE_RUN;
			else if (word == "die") state = Entity::STATE_DIE;
			else if (word == "spawn") state = Entity::STATE_SPAWN;
			else if (word == "fire_primary") state = Entity::STATE_FIRE_PRIMARY;
			else if (word == "fire_secondary") state = Entity::STATE_FIRE_SECONDARY;
		}
		else if (word == "play") {
			file_two >> word;
			if (word == "once") play = Animation::PLAY_ONCE;
			else if (word == "loop") play = Animation::PLAY_LOOP;
			else if (word == "swing") play = Animation::PLAY_SWING;
		}
		else if (word == "directions") {
			size_t n(0);
			file_two >> n;
			directions.clear();

			for (int i=0; i<n; i++) {
				file_two >> word;

				if (word == "E") directions.push_back(Entity::FACING_E);
				else if (word == "NE") directions.push_back(Entity::FACING_NE);
				else if (word == "N") directions.push_back(Entity::FACING_N);
				else if (word == "NW") directions.push_back(Entity::FACING_NW);
				else if (word == "W") directions.push_back(Entity::FACING_W);
				else if (word == "SW") directions.push_back(Entity::FACING_SW);
				else if (word == "S") directions.push_back(Entity::FACING_S);
				else if (word == "SE") directions.push_back(Entity::FACING_SE);
			}
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


	size_t dir_row = 0;
	for (size_t dir : directions) {
		std::vector<sf::Sprite> sprites;

		for (int f=0; f<frames; f++) {
			sf::IntRect rect(origin.x+frame_size.x*f, origin.y + frame_size.y*(dir_row), frame_size.x, frame_size.y);
			sprites.push_back(sf::Sprite(textures[texture_name], rect));
			sprites.back().setOrigin(frame_offset.x, frame_offset.y);
		}

		anim_set.animations[state][dir] = Animation(sprites, frame_time, play);
		dir_row++;
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

void Assets::loadShaders() {
	shaders["desaturate"].loadFromFile("res/shaders/desaturate.frag", sf::Shader::Fragment);
	shaders["rainbow"].loadFromFile("res/shaders/rainbow.frag", sf::Shader::Fragment);
	shaders["blur"].loadFromFile("res/shaders/blur.frag", sf::Shader::Fragment);
	shaders["predator"].loadFromFile("res/shaders/predator.frag", sf::Shader::Fragment);
	shaders["sobel"].loadFromFile("res/shaders/sobel.frag", sf::Shader::Fragment);
	shaders["pixelizer"].loadFromFile("res/shaders/pixelizer.frag", sf::Shader::Fragment);
	shaders["dream_vision"].loadFromFile("res/shaders/dream_vision.frag", sf::Shader::Fragment);
	shaders["bloom"].loadFromFile("res/shaders/bloom.frag", sf::Shader::Fragment);
	shaders["crt-vga"].loadFromFile("res/shaders/crt-vga.vert", "res/shaders/crt-vga.frag");
	shaders["crt-geom"].loadFromFile("res/shaders/crt-geom.vert", "res/shaders/crt-geom.frag");
	shaders["crt-mattias"].loadFromFile("res/shaders/crt-mattias.vert", "res/shaders/crt-mattias.frag");
}

sf::Shader& Assets::getShader(std::string name) {
	return 	shaders[name];
}
