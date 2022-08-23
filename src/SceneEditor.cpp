#include "SceneEditor.h"
#include "SUpdate.h"
#include "SDraw.h"

SceneEditor::SceneEditor(Game* g, std::string lp)
	:ScenePlay(g, GAME_SCENE::EDITOR, lp)
{
	game_state = GAME_EDIT;

	SceneEditor::init();
}

SceneEditor::~SceneEditor() {}

void SceneEditor::init() {
	music_fade_out = true;

	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::A, Action::MOVE_LEFT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::D, Action::MOVE_RIGHT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::N, Action::FIRE_PRIMARY);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::M, Action::FIRE_SECONDARY);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::P, Action::GAME_PAUSE);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Escape, Action::CHANGE_SCENE_MENU);

	interface.add(game->assets.getWidget("editor_menu_bar"));
	interface.add(game->assets.getWidget("editor_side_bar"));

	game->screen_tex.setView(gui_view);

	load_level(level_path);
}


void SceneEditor::update() {
	if (!isFading()) {
		ent_mgr.update();

		SUpdate::updatePosition(ent_mgr.getEntities(), level.map_ground.getBounds());
		sCollisionCheck();
		sCollisionSolve();
		sStateFacing();
		//sInterface();
		sAnimation();
		sGameState();
	}
	sView();

	game->screen_tex.draw(level.map_ground);
	SDraw::drawEntities(&game->screen_tex, ent_mgr.getEntities());


#ifdef DEBUG_GRID
	drawGrid();
#endif

#ifdef DEBUG_COLLISION_LAYER
	//drawCollisionLayer();
#endif

#ifdef DEBUG_DIRECTION
	drawDirectionVectors();
#endif

#ifdef DEBUG_ENTITY_POS
	drawEntityPosition();
#endif

	//change view in order to keep the interface relative to window
	{
		game->screen_tex.setView(gui_view);
		SDraw::drawInterface(&game->screen_tex, interface.getWidgets());
		game->screen_tex.setView(game->view);
	}

	frame_current++;
	sFade();
}
