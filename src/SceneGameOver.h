#ifndef SCENE_GAME_OVER
#define SCENE_GAME_OVER

#include "Scene.h"
#include "Interface.h"

class SceneGameOver: public Scene {
	WidgetVec interface;

	enum {
		SELECT_PLAY,
		SELECT_EXIT,
	};

	size_t selection = SELECT_PLAY;

	void init();

public:
	void update() override;
	void doAction(const Action* a) override;

	SceneGameOver();
	SceneGameOver(Game* g);
	~SceneGameOver();
};

#endif
