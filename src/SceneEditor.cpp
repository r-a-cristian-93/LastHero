#include "SceneEditor.h"
#include "SUpdate.h"
#include "SDraw.h"

SceneEditor::SceneEditor(Game* g, std::string lp)
	:ScenePlay(g, GAME_SCENE::EDITOR, lp)
{
	game_state = GAME_EDIT;
	game->window.setMouseCursorVisible(true);

	SceneEditor::init();
}

SceneEditor::~SceneEditor() {
	game->window.setMouseCursorVisible(false);
}

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


	{
		// SIDEBAR MOCKUP

		const std::map<size_t, Components>& recipes_env = game->assets.getRecipes(TAG::ENVIRONMENT);
		std::map<size_t, Components>::const_iterator recipe_it;
		sf::Vector2f max_size(100,100);
		int k = 1;


		for (recipe_it = recipes_env.begin(); recipe_it != recipes_env.end(); recipe_it++) {
			const Components& recipe = recipe_it->second;

			if (recipe.get<CAnimation>()) {
				sf::Sprite sprite = recipe.get<CAnimation>()->active_anim->getSprite();
				sprite.setOrigin(0,0);

				// scale down the sprite if it's to big
				sf::FloatRect current_size = sprite.getLocalBounds();
				if (current_size.width > max_size.x || current_size.height > max_size.y) {
					float s_x = max_size.x / current_size.width;
					float s_y = max_size.y / current_size.height;
					sf::Vector2f scale;

					if (s_x < s_y) {
						scale = {s_x, s_x};
					}
					else {
						scale = {s_y, s_y};
					}

					sprite.setScale(scale);
				}

				sf::FloatRect gb = sprite.getGlobalBounds();
				sf::Vector2i final_size(gb.width, gb.height);

				Widget widget;
				widget.setSize(final_size);
				widget.setBackground(sprite, {0, 0});
				widget.setPosAbs({85 - final_size.x / 2, 50 + k*100 - final_size.y / 2});

				interface.add(widget);
				k++;
			}
		}
	}

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
