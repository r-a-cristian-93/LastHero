#ifndef SCENE_GAME_OVER
#define SCENE_GAME_OVER

#include "Scene.h"

class SceneGameOver: public Scene {
	enum {
		SELECT_PLAY,
		SELECT_EXIT,
	};

	size_t selection = SELECT_PLAY;
	bool key_frame_rendered;

	void init();

public:
	void update() override;
	void doAction(const Action& a) override;

	SceneGameOver();
	~SceneGameOver();
};

#endif
