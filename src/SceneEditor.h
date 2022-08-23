#ifndef SCENE_EDITOR
#define SCENE_EDITOR

#include "ScenePlay.h"

class SceneEditor: public ScenePlay {
	void init();

public:
	void update() override;

	SceneEditor();
	SceneEditor(Game* g, std::string lp);
	~SceneEditor();
};


#endif

