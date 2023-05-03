#include "Level.h"
#include "SharedResources.h"


Level::Level()
{}

Level::Level(std::string _path) {
	path = _path;
	std::cout << "Loading level: " << path << std::endl;

	std::ifstream file(path);
	std::string word;

	std::string texture_name = "";
	int* level_layer;

	int frame, pos_x, pos_y, dir_x, dir_y;
	size_t enemy_name;
	size_t tag, state, facing;
	Action::Code code;
	Action::Type type;

	while (file >> word) {
		if (word == "_HEADER") {
			while (file>>word) {
				if (word == "_END") break;
				else if (word == "size")	{
					file >> map_size.x >> map_size.y;
					level_layer = new int[map_size.x*map_size.y];
				}
				else if (word == "tile_size") {
					file >> tile_size.x >> tile_size.y;
				}
				else if (word == "texture") {
					file >> texture_name;
				}
				else if (word == "bg_music") {
					file >> bg_music;
				}
			}
		}
		if (word == "_LAYER") {
			for (int t = 0; t < map_size.x*map_size.y; t++) {
				file >> level_layer[t];
			}
		}
		if (word == "_ACT") {
			while (file >> word) {
				if (word == "_END") break;
				else if (word == "code") {
					file >> word;
					if (word == "spawn") code = Action::SPAWN_ENTITY;
					else if (word  == "spawn_player") code = Action::SPAWN_PLAYER;
					else if (word  == "spawn_base") code = Action::SPAWN_BASE;
				}
				else if (word == "type") {
					file >> word;
					if (word == "start") type = Action::TYPE_START;
					else if (word == "end") type = Action::TYPE_END;
				}
				else if (word == "frame") file >> frame;
				else if (word == "entity") {
					file >> word;
					if (word == "enemy") tag = TAG::ENEMY;
					else if (word == "player") tag = TAG::PLAYER;
					else if (word == "base") tag = TAG::BASE;
					else if (word == "environment") tag = TAG::ENVIRONMENT;
					else if (word == "powerup") tag = TAG::POWERUP;
					else if (word == "fx") tag = TAG::FX;
					else {
						std::cout << "LOAD LEVEL: Tag \"" << word << "\" is not supported\n";
						exit(0);
					}

					file >> word;
					enemy_name = assets->getRecipeNameID(word);
				}
				else if (word == "pos") file >> pos_x >> pos_y;
				else if (word == "state") {
					file >> word;
					if (word == "idle") state = Entity::STATE_IDLE;
					else if (word == "run") state = Entity::STATE_RUN;
					else if (word == "die") state = Entity::STATE_DIE;
					else if (word == "spawn") state = Entity::STATE_SPAWN;
				}
				else if (word == "facing") {
					file >> word;
					if (word == "E") facing = Entity::FACING_E;
					else if (word == "NE") facing = Entity::FACING_NE;
					else if (word == "N") facing = Entity::FACING_N;
					else if (word == "NW") facing = Entity::FACING_NW;
					else if (word == "W") facing = Entity::FACING_W;
					else if (word == "SW") facing = Entity::FACING_SW;
					else if (word == "S") facing = Entity::FACING_S;
					else if (word == "SE") facing = Entity::FACING_SE;
				}
			}

			Action action(code, type);
			action.ent_tag = new size_t(tag);
			action.ent_name = new size_t(enemy_name);
			action.pos = new sf::Vector2f(pos_x, pos_y);
			action.state = new size_t(state);
			action.facing = new size_t(facing);

			actions.push_back(action);
		}
	}

	map_ground.setTexture(assets->getTexture(texture_name));
	map_ground.loadLevel(tile_size, level_layer, map_size);
	delete[] level_layer;
}
