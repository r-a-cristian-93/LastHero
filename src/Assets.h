#ifndef ASSETS
#define ASSETS

#include <vector>
#include <string>
#include <fstream>
#include "Entity.h"
#include "Border.h"

struct EntityDataset {
	size_t type;
	std::string name;
	int radius;
	float velocity;
	sf::Color fill;
	sf::Color outline;
	int out_thk;
	size_t vertices;
	int lifespan;
	int experience;
	int level;
	int stats_base[CStats::COUNT];
	int stats_per_level[CStats::COUNT];
	AnimationSet animation_set;
	sf::Color color_mod = {255, 255, 255, 255};
	std::string weapon_primary;
	std::string weapon_secondary;
	std::map<size_t, sf::Vector2f> projectile_spawn;
	std::map<size_t, sf::Vector2f> collision_offset;
	size_t prio = 0;
} ;

class Assets {
private:
	std::string file_path;
	std::ifstream file, file_two;
	std::string word;

	std::map<size_t, std::map<std::string, Components>> recipe;

	std::map<size_t, sf::Font> fonts;
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Sprite> sprites;
	std::map<std::string, Border> borders;
	std::map<std::string, sf::Shader> shaders;

	void loadEntities();
	void loadFonts();
	void loadGUI();
	void loadShaders();

	void loadEntity();
	void loadTexture();
	void loadSprite();
	void loadBorders();
	void loadBorderRepeatable(std::string border_name, size_t sprite_name, std::string texture_name);
	void loadAnimationSet(std::string path, AnimationSet& animation_set);
	void loadAnimation(AnimationSet& anim_set);
	sf::IntRect loadRect(std::ifstream& f);
	sf::Texture makeRepeatable(const sf::Texture& original, sf::IntRect& rect);
	void flipRectX(sf::IntRect& rect);
	void flipRectY(sf::IntRect& rect);

public:
	enum {
		NONE,
		FONT_COURIER,
		FONT_MILITARY
	};

	Assets();

	template<class T>
	const T* get(Components& c) {
		return std::get<T*>(c);
	}

	Components& getRecipe(size_t tag, std::string& recipe_name);
	Components& getRecipeRand(size_t tag);
	sf::Font& getFont(size_t name);
	Border& getBorder(std::string name);
	sf::Texture& getTexture(std::string name);
	sf::Sprite& getSprite(std::string name);
	sf::Shader& getShader(std::string name);

};

#endif
