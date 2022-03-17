#ifndef SCENE
#define SCENE

#include "Game.h"
#include "EntityManager.h"
#include "ActionManager.h"
#include "Action.h"

class Game;

class Scene {
protected:
	Game* game;
	EntityManager ent_mgr;
	sf::View gui_view;

	int frame_current;
	bool paused, has_ended;

	bool fade_in = false;
	bool fade_out = false;
	const size_t fade_in_frames = 60;
	const size_t fade_out_frames = 60;
	size_t current_fade_in = 0;
	size_t current_fade_out = fade_out_frames;
	size_t next_scene = 0;

	void init();

public:
	virtual void update() = 0;
	virtual void doAction(const Action* a) = 0;
	virtual void sFade();

	Scene();
	Scene(Game* g);
	virtual ~Scene() {}
};

#endif
