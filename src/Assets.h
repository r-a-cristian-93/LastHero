#ifndef ASSETS
#define ASSETS

#include <fstream>
#include "Common.h"
#include "Enums.h"
#include "Entity.h"
#include "Box.h"
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
	size_t p_delay = 0;
	size_t s_delay = 0;
	size_t p_sfx = 0;
	size_t s_sfx = 0;
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
	size_t sfx_hurt = 0;
	size_t sfx_die = 0;
	size_t sfx_spawn = 0;
	CBPowerup cb_powerup;


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

	std::vector<std::string> stages;
	std::map<size_t, std::map<size_t, Components>> recipe;
	std::map<size_t, Components*> all_recipes;
	std::map<std::string, size_t> recipe_name_id = {{"", NONE}};

	std::map<size_t, sf::Font> fonts;
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Sprite> sprites;
	std::map<std::string, Box> boxes;
	std::map<std::string, sf::Shader> shaders;
	std::map<size_t, sf::RenderStates> render_states;
	std::map<std::string, Widget> widgets;
	std::map<size_t, Widget> entities_icons;
	std::map<std::string, sf::Music> bg_music;

	std::map<size_t, sf::SoundBuffer> sound_buffers;
	std::map<std::string, size_t> sound_buffer_name_id = {{"", NONE}};

	void loadEntities();
	void loadFonts();
	void loadGUI();
	void loadWidgets();
	void loadShaders();
	void loadRenderStates();
	void loadSounds();
	void loadStages();

	void loadEntity();
	void loadTexture();
	void loadSprite();
	void loadBoxes();
	void loadWidget();
	void loadShader();
	void loadRenderState();
	void loadSong();
	void loadSound();
	void loadBoxRepeatable(std::string box_name, size_t sprite_name, std::string texture_name);
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
		FONT_PIXEL
	};

	Assets();
	~Assets();

	Components& getRecipe(size_t tag, size_t recipe_name);
	size_t getRecipeName(size_t tag);
	size_t getRecipeNameID(std::string entity_name);
	const std::map<size_t, Components>& getRecipes(size_t tag);
	size_t getScorePoints(size_t name_id);
	sf::Font& getFont(size_t name);
	Box& getBox(std::string name);
	sf::Texture& getTexture(std::string name);
	sf::Sprite& getSprite(std::string name);
	sf::Shader& getShader(std::string name);
	sf::RenderStates& getRenderStates(size_t render_style);
	Widget& getEntityIcon(size_t name_id);
	Widget& getWidget(std::string name);
	sf::Music& getSound(std::string name);
	sf::SoundBuffer& getSoundBuffer(size_t id);
	sf::SoundBuffer& getSoundBuffer(std::string name);
	size_t getSoundBufferNameID(std::string sound_name);
	std::vector<std::string>& getStages();
};

#endif
