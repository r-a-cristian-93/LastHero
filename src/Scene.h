#ifndef SCENE
#define SCENE

#include "Game.h"
#include "EntityManager.h"
#include "ActionManager.h"
#include "Action.h"

class Game;

typedef size_t FadeType;

class Scene {
protected:
	Game* game;
	EntityManager ent_mgr;
	sf::View gui_view;
	std::string name;

	int frame_current;
	bool paused, has_ended;

	bool music_fade_in;
	bool music_fade_out;

	enum {
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_COUNT
	};

	void init();

private:
	FadeType fade = FADE_NONE;
	size_t fade_frames[FADE_COUNT] = {0, 60, 60};
	size_t current_fade_frames[FADE_COUNT] = {0, 0, 60};
	size_t next_scene = 0;

public:
	virtual void update() = 0;
	virtual void doAction(const Action* a) = 0;
	void sFade();
	void setFade(FadeType fade, size_t frames);
	void setFade(FadeType fade, size_t frames, size_t scene);
	bool isFading();
	const FadeType getCurrentFade();

	Scene();
	Scene(Game* g);
	virtual ~Scene();
};

#endif
