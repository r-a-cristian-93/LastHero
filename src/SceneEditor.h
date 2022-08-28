#ifndef SCENE_EDITOR
#define SCENE_EDITOR

#include "ScenePlay.h"

class SceneEditor: public ScenePlay {
	enum {
		SELECT_NEW = 0,
		SELECT_LOAD,
		SELECT_SAVE,
		SELECT_TERRAIN,
		SELECT_ENVIRONMENT,
		SELECT_CREATURES,

	};

	sf::Color mod_highlight = {220, 220, 220};
	sf::Color mod_dark = {100, 100, 100};

	void init();

public:
	void update() override;
	void doAction(const Action& a) override;

	SceneEditor();
	SceneEditor(Game* g, std::string lp);
	~SceneEditor();
};


#endif

