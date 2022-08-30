#ifndef SCENE
#define SCENE

#include "SharedResources.h"
#include "Game.h"
#include "EntityManager.h"
#include "ActionManager.h"
#include "Action.h"
#include "Interface.h"

class Game;

typedef size_t FadeType;
typedef size_t SceneType;

class Scene {
protected:
	EntityManager ent_mgr;

	int frame_current;
	bool paused, has_ended;
	bool skip_key_frames = false;

	void init();

public:
	SceneType scene_type = GAME_SCENE::NONE;
	Interface interface;
	sf::View gui_view;

	FadeType fade = FADE::NONE;
	unsigned char current_fade_frames[FADE::COUNT] = {0};
	const unsigned int* fade_frames = nullptr;
	bool music_fade_in;
	bool music_fade_out;
	size_t next_scene = 0;

	virtual void update() = 0;
	virtual void doAction(const Action& a) = 0;

	void setFade(FadeType fade);
	void setFade(FadeType fade, size_t scene);
	bool isFading();
	const FadeType getCurrentFade();

	Scene();
	Scene(SceneType scene_type);
	virtual ~Scene();

};

#endif
