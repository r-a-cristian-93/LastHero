#include <iostream>

#include "Editor.h"
#include "SceneEditor.h"

Editor::Editor()
{
	init();
}

void Editor::init() {
	//load texture after creating the window causes sementation fault;
	screen_tex->create(app_conf->game_w, app_conf->game_h);
	screen_sprite->setTextureRect({0, 0, app_conf->game_w, app_conf->game_h});

	cfg_mgr->applySettings(*app_conf);

	setScene(GAME_SCENE::EDITOR);
}

void Editor::setScene(size_t id) {
	if (current_scene) {
		if (current_scene->scene_type == GAME_SCENE::EDITOR) setStyleEditor();
	}

	delete current_scene;
	act_mgr->reset();

	switch (id) {
		case GAME_SCENE::EDITOR:
			current_scene = new SceneEditor(assets->getStages()[game_stats->next_stage]);
			setStyleEditor();
		break;
		default:
			std::cout << __FILE__ << ":" << __LINE__ << " ERROR : Scene id " << id << " not handled\n";
		break;
	}
}

Editor::~Editor() {}
