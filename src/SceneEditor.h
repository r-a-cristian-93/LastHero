#ifndef SCENE_EDITOR
#define SCENE_EDITOR

#include "Scene.h"
#include "Level.h"

class SceneEditor: public Scene {
	std::string level_path;
	Level level;

	Camera cam;
	Interface interface;
	GameState game_state;

	void init();
	void load_level(std::string path);

	void spawnEntity(size_t tag, size_t recipe_name, sf::Vector2f& pos, size_t state, size_t facing);
	void spawnEntity(size_t tag, size_t recipe_name, std::shared_ptr<Entity> owner, sf::Vector2f& pos, size_t state, size_t facing);

	void sStateFacing();
	void sInterface();
	void sAnimation();
	void sView();

	void drawCollisionLayer();
	void drawDirectionVectors();
	void drawGrid();
	void drawEntityPosition();

public:
	void update() override;
	void doAction(const Action& a) override;

	SceneEditor();
	SceneEditor(Game* g);
	~SceneEditor();
};


#endif

