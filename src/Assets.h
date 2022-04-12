#ifndef ASSETS
#define ASSETS

#include <vector>
#include <string>
#include <fstream>
#include "Common.h"
#include "Enums.h"
#include "Entity.h"
#include "Border.h"
#include "Interface.h"

struct EntityDataset {
	size_t tag;
	size_t name_id = 0;
	float velocity = 0;
	sf::Color fill = {255, 255, 255, 111};
	sf::Color outline {0, 0, 0, 255};
	int out_thk = 0;
	int score_points = 0;
	int lifespan = 0;
	int experience = 0;
	int level = 0;
	int* stats_base = nullptr;
	int* stats_per_level = nullptr;
	AnimationSet animation_set;
	sf::Color color_mod = {255, 255, 255, 255};
	size_t weapon_primary = 0;
	size_t weapon_secondary = 0;
	size_t p_tag = 0;
	size_t s_tag = 0;
	size_t primary_cooldown = 0;
	size_t secondary_cooldown = 0;
	size_t p_rounds = 0;
	size_t s_rounds = 0;
	std::map<size_t, sf::Vector2f> projectile_spawn;
	size_t prio = 0;
	std::vector<HitBox> hitbox;
	std::string icon = "";
	std::vector<BCondition> cb_fire_primary;
	std::vector<BCondition> cb_fire_secondary;
	size_t cb_patrol = 0;
	size_t cb_patrol_dist = 0;
	std::vector<BCondition> cb_chase;
	std::vector<Fx> c_fx;

	~EntityDataset() {
		delete[] stats_base;
		delete[] stats_per_level;
	}
} ;

class Assets {
private:
	std::string file_path;
	std::ifstream file, file_two;
	std::string word;

	std::map<size_t, std::map<size_t, Components>> recipe;
	std::map<size_t, Components*> all_recipes;
	std::map<std::string, size_t> recipe_name_id = {{"", NONE}};
	std::map<size_t, sf::Sprite*> icon_small;

	std::map<size_t, sf::Font> fonts;
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Sprite> sprites;
	std::map<std::string, Border> borders;
	std::map<std::string, sf::Shader> shaders;
	std::map<std::string, Widget> widgets;

	void loadEntities();
	void loadFonts();
	void loadGUI();
	void loadWidgets();
	void loadShaders();

	void loadEntity();
	void loadTexture();
	void loadSprite();
	void loadBorders();
	void loadWidget();
	void loadBorderRepeatable(std::string border_name, size_t sprite_name, std::string texture_name);
	void loadAnimationSet(std::string path, AnimationSet& animation_set);
	void loadAnimation(AnimationSet& anim_set);
	sf::IntRect loadRect(std::ifstream& f);
	sf::Texture makeRepeatable(const sf::Texture& original, sf::IntRect& rect);
	void flipRectX(sf::IntRect& rect);
	void flipRectY(sf::IntRect& rect);

	size_t parseTag(const std::string& word);
	size_t parseTrigger(const std::string& word);

public:
	enum {
		NONE = 0,
		FONT_COURIER,
		FONT_MILITARY
	};

	Assets();
	~Assets();

	Components& getRecipe(size_t tag, size_t recipe_name);
	size_t getRecipeName(size_t tag);
	size_t getRecipeNameID(std::string entity_name);
	size_t getScorePoints(size_t name_id);
	sf::Font& getFont(size_t name);
	Border& getBorder(std::string name);
	sf::Texture& getTexture(std::string name);
	sf::Sprite& getSprite(std::string name);
	sf::Sprite& getIconSmall(size_t name_id);
	sf::Shader& getShader(std::string name);
	Widget& getWidget(std::string name);
};

#endif
