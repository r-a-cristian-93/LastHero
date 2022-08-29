#ifndef GAME
#define GAME

#include <deque>

#include "SharedResources.h"
#include "Entity.h"
#include "EntityManager.h"
#include "ActionManager.h"
#include "SoundManager.h"
#include "Common.h"
#include "Assets.h"
#include "Scene.h"
#include "AppConfig.h"

#include "Profiler.h"

class Scene;

typedef std::map<size_t, size_t> KillsMap;

class Game {
private:
	Scene* current_scene;
	size_t next_scene;

	size_t next_stage;
	size_t prev_stage;

	void init();

	void sUserInput();
	void sChangeScene();

	void handleUIEvent(sf::Event& event, WidgetVec& widgets);

	void setScene(size_t id);

public:
	KillsMap kills_per_enemy;
	KillsMap new_kills_per_enemy;

	bool running;
	sf::RenderWindow window;
	sf::RenderTexture screen_tex;
	sf::Sprite screen_sprite;
	sf::View view;

	AppConfig editor_conf;
	ActionManager act_mgr;
	SoundManager snd_mgr;
	Assets assets;

	void addKills(std::map<size_t, size_t> kills_per_enemy);
	bool stageNext();
	void stageReset();
	size_t stageCurrent();
	size_t stagePrev();
	size_t stagesCount();
	void run();

	void setNextScene(size_t id);
	void fit(sf::Sprite& sprite);
	void setStyleEditor();
	void setStyleGame();

	void applySettings(AppConfig& conf);

	Game();
	~Game();
};

#endif
