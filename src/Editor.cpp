#include <iostream>

#include "Editor.h"
#include "SceneEditor.h"

Editor::Editor()
{
	init();
}

void Editor::init() {
	//load texture after creating the window causes sementation fault;
	screen_tex->create(app_conf->modes[app_conf->current_mode_id].width, app_conf->modes[app_conf->current_mode_id].height);
	screen_sprite->setTextureRect({0, 0, app_conf->modes[app_conf->current_mode_id].width, app_conf->modes[app_conf->current_mode_id].height});

	window->create(app_conf->modes[app_conf->current_mode_id], app_conf->window_name, app_conf->window_style);
	window->setFramerateLimit(app_conf->max_fps);
	window->setKeyRepeatEnabled(false);
	window->setMouseCursorVisible(false);

	setScene(GAME_SCENE::EDITOR);
}

void Editor::setScene(size_t id) {
	delete current_scene;
	act_mgr->reset();

	switch (id) {
		case GAME_SCENE::EDITOR:
			current_scene = new SceneEditor(assets->getStages()[game_stats->next_stage]);
		break;
		default:
			std::cout << __FILE__ << ":" << __LINE__ << " ERROR : Scene id " << id << " not handled\n";
		break;
	}
}

Editor::~Editor() {}
