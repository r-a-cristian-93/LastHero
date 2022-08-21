#include "SceneEditor.h"
#include "SUpdate.h"
#include "SDraw.h"

SceneEditor::SceneEditor(Game* g)
	:Scene(g, GAME_SCENE::EDITOR)
{
	init();
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
}



void SceneEditor::update() {
	if (!isFading()) {
		ent_mgr.update();

		//sStateFacing();
		//sInterface();
		//sAnimation();
//		sGameState();
	}
	//sView();

	game->screen_tex.draw(level.map_ground);
	SDraw::drawEntities(&game->screen_tex, ent_mgr.getEntities());


#ifdef DEBUG_GRID
	//drawGrid();
#endif

#ifdef DEBUG_COLLISION_LAYER
	//drawCollisionLayer();
#endif

#ifdef DEBUG_DIRECTION
	//drawDirectionVectors();
#endif

#ifdef DEBUG_ENTITY_POS
	//drawEntityPosition();
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

void SceneEditor::doAction(const Action& a) {
	if (*a.type == Action::TYPE_START && getCurrentFade() != FADE::OUT) {
		switch (*a.code) {
			case Action::CHANGE_SCENE_MENU:
				setFade(FADE::OUT, GAME_SCENE::MENU);
			break;
			default:
			break;
		}
	}
	if (*a.type == Action::TYPE_END) {
		switch (*a.code) {
			default:
			break;
		}
	}
}

