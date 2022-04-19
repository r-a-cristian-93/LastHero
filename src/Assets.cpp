#include <fstream>
#include <iostream>
#include "Common.h"
#include "Assets.h"
#include "Settings.h"

Assets::Assets() {
	loadGUI();
	loadWidgets();
	loadEntities();
	loadFonts();
	loadShaders();
}

Components& Assets::getRecipe(size_t tag, size_t name_id) {
	return recipe[tag][name_id];
}

size_t Assets::getScorePoints(size_t name_id) {
	if (all_recipes.count(name_id)){
		if (all_recipes[name_id]->get<CScore>()) {
			return all_recipes[name_id]->get<CScore>()->score;
		}
	}

	return 0;
}

size_t Assets::getRecipeName(size_t tag) {
	std::map<size_t, Components>::iterator it = recipe[tag].begin();
	std::advance(it, rand() % recipe[tag].size());
	return (it->first);
}

size_t Assets::getRecipeNameID(std::string entity_name) {
	if (recipe_name_id.count(entity_name)) {
		return recipe_name_id[entity_name];
	}

	std::cout << "recipe_name_id \"" << entity_name << "\" not found.\n";
	exit(0);
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

sf::Sprite& Assets::getIconSmall(size_t name_id) {
	if (icon_small.count(name_id)) {
		return *icon_small[name_id];
	}

	std::cout << "Entity with name_id \"" << name_id << "\" might not have a small icon.\n";
	exit(0);
}

Widget& Assets::getWidget(std::string name) {
	if (widgets.count(name)) return widgets.at(name);

	std::cout << "Widget " << name << " could not be found.\n";
	exit(0);
}

void Assets::loadEntities() {
	file.open("res/entities.cfg");

	while (file >> word) {
		if (word == "_ENT") {
			loadEntity();
		}
	}

	file.close();

#ifdef DEBUG_RECIPE_NAME_ID
	std::cout << "DEBUG_RECIPE_NAME_ID";
	std::map<std::string, size_t>::iterator it;
	for (it = recipe_name_id.begin(); it != recipe_name_id.end(); it++) {
		std::cout << it->first << " " << it->second << std::endl;
	}
#endif

}

void Assets::loadEntity() {
	EntityDataset data_ent;

	while (file >> word) {
		if (word == "_END") break;
		else if (word == "type") {
			file >> word;
			data_ent.tag = parseTag(word);
		}
		else if (word == "name") {
			file >> word;

			// register recipe id
			if (!recipe_name_id.count(word)) {
				size_t new_id = recipe_name_id.size();
				recipe_name_id[word] = new_id;
			}

			data_ent.name_id = getRecipeNameID(word);
		}
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
		else if (word == "score_points") file >> data_ent.score_points;
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
			data_ent.p_tag = parseTag(word);

			file >> word;
			data_ent.weapon_primary = getRecipeNameID(word);
			file >> data_ent.primary_cooldown;
			file >> data_ent.p_rounds;
			file >> data_ent.p_delay;
		}
		else if (word == "weapon_secondary") {
			file >> word;
			data_ent.s_tag = parseTag(word);

			file >> word;
			data_ent.weapon_secondary = getRecipeNameID(word);
			file >> data_ent.secondary_cooldown;
			file >> data_ent.s_rounds;
			file >> data_ent.s_delay;
		}
		else if (word == "projectile_spawn") {
			for (int i=1; i<=8; i++) {
				float dx, dy;
				file >> dx >> dy;
				data_ent.projectile_spawn[i].x = dx;
				data_ent.projectile_spawn[i].y = dy;
			}
		}
		else if (word == "fx") {
			Fx fx;

			file >> word;
			if (word == "tr_die") fx.trigger = TR::DIE;
			else if (word == "tr_spawn") fx.trigger = TR::SPAWN;
			else {
				std::cout << "CFx trigger " << word << " is not supported.\n";
				exit(0);
			}

			file >> word;
			fx.id = getRecipeNameID(word);

			if (fx.id) 	data_ent.c_fx.push_back(fx);
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
		else if (word == "icon") {
			file >> data_ent.icon;
		}
		else if (word == "behaviour") {
			file >> word;
			if (word == "fire_primary") {
				BCondition bc;
				file >> word;
				bc.trigger = parseTrigger(word);
				file >> bc.data_start;
				data_ent.cb_fire_primary.push_back(bc);
			}
			else if (word == "fire_secondary") {
				BCondition bc;
				file >> word;
				bc.trigger = parseTrigger(word);
				file >> bc.data_start;
				data_ent.cb_fire_secondary.push_back(bc);
			}
			else if (word == "patrol") {
				file >> word;

				if (word == "horizontal") data_ent.cb_patrol = CBPatrol::PATROL_HORIZONTAL;
				else if (word == "vertical") data_ent.cb_patrol = CBPatrol::PATROL_VERTICAL;
				else if (word == "square") data_ent.cb_patrol = CBPatrol::PATROL_SQUARE;
				else {
					std::cout << "Patrol type " << word << " is not supported.\n";
					exit(0);
				}

				file >> data_ent.cb_patrol_dist;
			}
			else if (word == "chase") {
				BCondition bc;
				file >> word;
				bc.trigger = parseTrigger(word);
				file >> bc.data_start >> bc.data_stop;

				if (bc.trigger && bc.data_start && bc.data_stop) {
					data_ent.cb_chase.push_back(bc);
				}
				else {
					std::cout << "Behaviour chase can not contain 0 values\n";
					exit(0);
				}
			}
			else {
				std::cout << "Behaviour " << word << " is not supported.\n";
				exit(0);
			}
		}
	}

	switch (data_ent.tag) {
		case TAG::PLAYER:
		case TAG::BASE:
		case TAG::ENEMY:
		case TAG::ENVIRONMENT:
		case TAG::PROJECTILE:
		case TAG::MISSLE:
		case TAG::FX:
		{
			// add Transform
			recipe[data_ent.tag][data_ent.name_id].add<CTransform>(new CTransform(data_ent.velocity));

			// add CShape
			sf::CircleShape shape;
			shape.setFillColor(data_ent.fill);
			recipe[data_ent.tag][data_ent.name_id].add<CShape>(new CShape(shape));

			// add CScore		{
			recipe[data_ent.tag][data_ent.name_id].add<CScore>(new CScore(data_ent.score_points));

			// add CStats
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
				recipe[data_ent.tag][data_ent.name_id].add<CStats>(new CStats(stats));
			}

			// add CLifespan
			if (data_ent.lifespan) {
				recipe[data_ent.tag][data_ent.name_id].add<CLifespan>(new CLifespan(data_ent.lifespan));
			}

			// add CCollsion
			if (!data_ent.hitbox.empty()) {
				CCollision collision;
				collision.hitbox = data_ent.hitbox;
				recipe[data_ent.tag][data_ent.name_id].add<CCollision>(new CCollision(collision));
			}

			// add CAnimation
			if (data_ent.animation_set.animations.size() > 0) {
				recipe[data_ent.tag][data_ent.name_id].add<CAnimation>(new CAnimation(data_ent.animation_set));
				recipe[data_ent.tag][data_ent.name_id].get<CAnimation>()->anim_set.setColorMod(data_ent.color_mod);
				recipe[data_ent.tag][data_ent.name_id].get<CAnimation>()->prio = data_ent.prio;
			}

			// add CWeapon
			if (data_ent.weapon_primary!=NONE || data_ent.weapon_secondary!=NONE) {
				CWeapon weapon(data_ent.weapon_primary, data_ent.weapon_secondary);
				weapon.p_tag = data_ent.p_tag;
				weapon.s_tag = data_ent.s_tag;
				weapon.projectile_spawn = data_ent.projectile_spawn;
				weapon.p_cooldown = data_ent.primary_cooldown;
				weapon.s_cooldown = data_ent.secondary_cooldown;
				weapon.p_rounds = data_ent.p_rounds;
				weapon.s_rounds = data_ent.s_rounds;
				weapon.p_delay = data_ent.p_delay;
				weapon.s_delay = data_ent.s_delay;
				weapon.p_delay_current = data_ent.p_delay;
				weapon.s_delay_current = data_ent.s_delay;
				recipe[data_ent.tag][data_ent.name_id].add<CWeapon>(new CWeapon(weapon));
				//add CInput
			recipe[data_ent.tag][data_ent.name_id].add<CInput>(new CInput());
			}

			// add CFx
			if (!data_ent.c_fx.empty()) {
				recipe[data_ent.tag][data_ent.name_id].add<CFx>(new CFx(data_ent.c_fx));
			}

			// add CBFire
			if (!data_ent.cb_fire_primary.empty() || !data_ent.cb_fire_secondary.empty()) {
				recipe[data_ent.tag][data_ent.name_id].add<CBFire>(new CBFire(data_ent.cb_fire_primary, data_ent.cb_fire_secondary));
			}

			// add CBPatrol
			if (data_ent.cb_patrol) {
				recipe[data_ent.tag][data_ent.name_id].add<CBPatrol>(new CBPatrol(data_ent.cb_patrol, data_ent.cb_patrol_dist, {0,0}));
			}

			//add CBChase
			if (!data_ent.cb_chase.empty()) {
				recipe[data_ent.tag][data_ent.name_id].add<CBChase>(new CBChase(data_ent.cb_chase));
			}

			// add icon
			if (!data_ent.icon.empty()) {
				icon_small[data_ent.name_id] = &getSprite(data_ent.icon);
			}

			// add a reference of the recipe for ease of access;
			all_recipes[data_ent.name_id] = &recipe[data_ent.tag][data_ent.name_id];
		}
		break;
		default:
			std::cout << "Invalid entity tag " << data_ent.tag << std::endl;
			exit(0);
		break;
	}
}


size_t Assets::parseTag(const std::string& word) {
	if (word == "player") return TAG::PLAYER;
	else if (word == "base") return TAG::BASE;
	else if (word == "environment") return TAG::ENVIRONMENT;
	else if (word == "projectile") return TAG::PROJECTILE;
	else if (word == "missle") return TAG::MISSLE;
	else if (word == "enemy") return TAG::ENEMY;
	else if (word == "fx") return TAG::FX;
	else {
		std::cout << "Invalid entity tag \"" << word << "\".\n";
		exit(0);
	}
}

size_t Assets::parseTrigger(const std::string& word) {
	if (word == "tr_cont") return TR::CONTINUOUS;
	else if (word == "tr_rand") return TR::RANDOM;
	else if (word == "tr_cont") return TR::CONTINUOUS;
	else if (word == "tr_player_low_hp") return TR::PLAYER_LOW_HP;
	else if (word == "tr_player_nearby") return TR::PLAYER_NEARBY;
	else if (word == "tr_base_low_hp") return TR::BASE_LOW_HP;
	else if (word == "tr_base_nearby") return TR::BASE_NEARBY;
	else if (word == "tr_base_not_protected") return TR::BASE_NOT_PROTECTED;
	else {
		std::cout << "Invalid trigger \"" << word << "\".\n";
		exit(0);
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
			else if (word == "hit") state = Entity::STATE_HIT;
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

	if (!fonts[FONT_ARCADE_CLASSIC].loadFromFile("res/arcade_classic.ttf")) {
		std::cout << "Can't load font MILITARY\n";
	}

	if (!fonts[FONT_ARCADE_KARMATIC].loadFromFile("res/arcade_karmatic.ttf")) {
		std::cout << "Can't load font MILITARY\n";
	}

	if (!fonts[FONT_PRESS_START].loadFromFile("res/press_start_2p.ttf")) {
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

void Assets::loadWidgets() {
	file_path = "res/interface/widgets.cfg";
	file.open(file_path);

	while (file >> word) {
		if (word == "_END") break;
		else if (word == "_WIDGET") {
			loadWidget();
		}
	}

	file.close();
}

void Assets::loadWidget() {
	std::string name(""), type(""), bg_sprite(""), bg_tex(""), bg_tex_hover(""), border(""), border_hover("");
	sf::Vector2i size, pos_rel, pos_abs;
	sf::Vector2i spr_offset;
	int tex_offset(0), w(0), h(0), font_size(0);
	size_t font_id(NONE), link(0);
	std::vector<std::string> childs;
	sf::Color text_color(255, 255, 255);

	while (file >> word) {
		if (word == "_END") break;
		else if (word == "name") file >> name;
		else if (word == "type") file >> type;
		else if (word == "size") file >> size.x >> size.y;
		else if (word == "link") {
			file >> word;
			if (word == "player_health") link = Widget::LINK_PLAYER_HP;
			else if (word == "base_health") link = Widget::LINK_BASE_HP;
			else if (word == "total_kills") link = Widget::LINK_TOTAL_KILLS;
			else {
				std::cout << "Invalid link: " << word << std::endl;
				exit(0);
			}
		}
		else if (word == "pos_rel") file >> pos_rel.x >> pos_rel.y;
		else if (word == "pos_abs") file >> pos_abs.x >> pos_abs.y;
		else if (word == "bg_sprite") file >> bg_sprite >> spr_offset.x >> spr_offset.y;
		else if (word == "bg_tex") file >> bg_tex >> tex_offset;
		else if (word == "bg_tex_hover") file >> bg_tex_hover >> tex_offset;
		else if (word == "border") file >> border;
		else if (word == "border_hover") file >> border_hover;
		else if (word == "font") {
			file >> word >> font_size;
			if (word == "courier") font_id = FONT_COURIER;
			else if (word == "military") font_id = FONT_MILITARY;
			else if (word == "classic") font_id = FONT_ARCADE_CLASSIC;
			else if (word == "karmatic") font_id = FONT_ARCADE_KARMATIC;
			else if (word == "2p") font_id = FONT_PRESS_START;
			else {
				std::cout << "Invalid font specified \"" << word << "\".\n";
				exit(0);
			}
		}
		else if (word == "text_color") {
			int r, g, b;
			file >> r, g, b;
			text_color = sf::Color(r, g, b);
		}
		else if (word == "child") {
			file >> word;
			childs.push_back(word);
		}
	}

	if (!name.empty() && !type.empty()) {
		Widget& widget = widgets[name];

		if (type == "box") {
			widget.setSize(size);
			if (!bg_sprite.empty()) widget.setBackground(sprites[bg_sprite], spr_offset);
			if (!bg_tex.empty()) widget.setBackground(textures[bg_tex], tex_offset);
			if (!border.empty()) widget.setBorder(borders[border]);
		}
		else if (type == "text") {
			widget.setSize(size);
			widget.setText("TEXT", fonts[font_id], font_size);
			widget.setTextColor(text_color);
			widget.link = link;
		}
		else {
			std::cout << "Invalid widget type \"" << type << "\".\n";
			exit(0);
		}

		widget.setPosRel(pos_rel);
		widget.setPosAbs(pos_abs);

		if (childs.size() > 0) {
			for (size_t i=0; i<childs.size(); i++) {
				if (widgets.find(childs[i]) != widgets.end()) {
					widget.addChild(widgets[childs[i]]);
				}
				else {
					std::cout << "Widget \"" << childs[i] << "\" does not exist.\n";
					exit(0);
				}
			}
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
		delete path;
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
	shaders["crt-easy"].loadFromFile("res/shaders/crt-easy.vert", "res/shaders/crt-easy.frag");
	shaders["crt-fake"].loadFromFile("res/shaders/crt-fake.vert", "res/shaders/crt-fake.frag");
}

sf::Shader& Assets::getShader(std::string name) {
	return 	shaders[name];
}

Assets::~Assets() {}

