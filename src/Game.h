#ifndef GAME
#define GAME

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

typedef std::map<size_t, size_t> KillsMap;

class Game {
private:
	Scene* current_scene;
	std::map<size_t, Scene*> scenes;

	void init(std::string file_name);

	void sUserInput();

public:
	KillsMap kills_per_enemy = {{8,3}, {9,4}, {10,6}, {12,2}};
	KillsMap new_kills_per_enemy = {{12,2}};

	enum {
		NONE,
		GAME_SCENE_MENU,
		GAME_SCENE_PLAY,
		GAME_SCENE_OVER,
		GAME_SCENE_SCORE,
	};

	bool running;
	sf::RenderWindow window;
	sf::RenderTexture screen_tex;
	sf::Sprite screen_sprite;
	sf::View view;
	AppConfig app_conf;
	ActionManager act_mgr;
	Assets* assets;

	void setScene(size_t id);
	void addKills(std::map<size_t, size_t> kills_per_enemy);
	void run();

	Game(std::string file_name);
	~Game();
};

#endif
