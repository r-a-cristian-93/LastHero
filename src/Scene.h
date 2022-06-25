#ifndef SCENE
#define SCENE

#include "Game.h"
#include "EntityManager.h"
#include "ActionManager.h"
#include "Action.h"

class Game;

typedef size_t FadeType;
typedef size_t SceneType;

class Scene {
protected:
	Game* game;
	EntityManager ent_mgr;
	sf::View gui_view;
	SceneType scene_type = GAME_SCENE::NONE;

	const unsigned int* fade_frames = nullptr;
	int frame_current;
	bool paused, has_ended;
	bool skip_key_frames = false;

	bool music_fade_in;
	bool music_fade_out;

	void init();

private:
	FadeType fade = FADE::NONE;
	unsigned char current_fade_frames[FADE::COUNT] = {0};

	size_t next_scene = 0;

public:
	virtual void update() = 0;
	virtual void doAction(const Action* a) = 0;
	void sFade();
	void setFade(FadeType fade);
	void setFade(FadeType fade, size_t scene);
	bool isFading();
	const FadeType getCurrentFade();

	Scene();
	Scene(Game* g, SceneType scene_type);
	virtual ~Scene();
};

#endif
