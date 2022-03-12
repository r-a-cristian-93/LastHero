#ifndef GAME
#define GAME

//#define DEBUG_PROFILING
//#define DRAW_COLLISION_BOX

#include <SFML/Graphics.hpp>
#include <string>
#include <deque>

#include "Entity.h"
#include "EntityManager.h"
#include "ActionManager.h"
#include "ActionStream.h"
#include "Common.h"
#include "Assets.h"
#include "Scene.h"

#include "Profiler.h"

class Scene;

class Game {
private:
	bool running;
	Scene* current_scene;
	std::map<size_t, Scene*> scenes;

	void init(std::string file_name);

	void sUserInput();

public:
	enum {
		NONE,
		SCENE_MENU,
		SCENE_PLAY
	};

	sf::RenderWindow window;
	sf::RenderTexture screen_tex;
	sf::Sprite screen_sprite;
	sf::View view;
	AppConfig app_conf;
	ActionManager act_mgr;
	Assets* assets;

	void setScene(size_t id);
	void run();

	Game(std::string file_name);
	~Game();
};

#endif
