#include "SceneEditor.h"
#include "SUpdate.h"
#include "SDraw.h"
#include <cmath>

SceneEditor::SceneEditor(std::string lp)
	:ScenePlay(GAME_SCENE::EDITOR, lp)
{
	window->setMouseCursorVisible(true);

	SceneEditor::init();
}

SceneEditor::~SceneEditor() {
	window->setMouseCursorVisible(false);
}

void SceneEditor::init() {
	music_fade_out = true;

	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::A, Action::MOVE_LEFT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::D, Action::MOVE_RIGHT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Escape, Action::GAME_EXIT);
	act_mgr->registerAction(ActionManager::DEV_MOUSE, sf::Mouse::Button::Left, Action::LEFT_CLICK);

	gui_view.reset(sf::FloatRect(0, 0, app_conf->modes[app_conf->current_mode_id].width, app_conf->modes[app_conf->current_mode_id].height));

	interface.add(assets->getWidget("editor_menu_bar"));

	Widget& side_bar = assets->getWidget("editor_side_bar");
	side_bar.addChild(assets->getWidget("tag_terrain"));
	side_bar.addChild(assets->getWidget("tag_environment"));
	side_bar.addChild(assets->getWidget("tag_creatures"));
	side_bar.addChild(assets->getWidget("entity_scroll"));

	interface.add(assets->getWidget("editor_side_bar"));


	{
		// SIDEBAR MOCKUP

		const std::map<size_t, Components>& recipes_env = assets->getRecipes(TAG::ENVIRONMENT);
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

	screen_tex->setView(gui_view);

	load_level(level_path);
}


void SceneEditor::update() {
	if (!isFading()) {
		ent_mgr.update();

		SUpdate::updatePosition(ent_mgr.getEntities(), level.map_ground.getBounds());
		sCollisionCheck();
		sCollisionSolve();
		sStateFacing();
		sInterface();
		sAnimation();
		sGameState();
	}
	sView();

	screen_tex->draw(level.map_ground);
	SDraw::drawEntities(&*screen_tex, ent_mgr.getEntities());


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
		screen_tex->setView(gui_view);
		SDraw::drawInterface(&*screen_tex, interface.getWidgets());
		screen_tex->setView(*game_view);
	}

	frame_current++;
}


void SceneEditor::doAction(const Action& a) {
	if (*a.type == Action::TYPE_START && getCurrentFade() != FADE::OUT) {
		switch (*a.code) {
			case Action::MOVE_UP:
				player->get<CInput>()->up = true;
			break;
			case Action::MOVE_LEFT:
				player->get<CInput>()->left = true;
			break;
			case Action::MOVE_DOWN:
				player->get<CInput>()->down = true;
			break;
			case Action::MOVE_RIGHT:
				player->get<CInput>()->right = true;
			break;
			case Action::GAME_EXIT:
				snd_mgr->playSound("menu_cancel");
				setFade(FADE::OUT, GAME_SCENE::EXIT);
			break;
			case Action::SPAWN_ENTITY:
				spawnEntity(*a.ent_tag, *a.ent_name, *a.pos, *a.state, *a.facing);
			break;
			case Action::SPAWN_PLAYER:
				spawnEntity(*a.ent_tag, *a.ent_name, *a.pos, *a.state, *a.facing);
				ent_mgr.update();
				player = ent_mgr.getEntities(TAG::PLAYER)[0];
			break;
			case Action::SPAWN_BASE:
				spawnEntity(*a.ent_tag, *a.ent_name, *a.pos, *a.state, *a.facing);
				ent_mgr.update();
				base = ent_mgr.getEntities(TAG::BASE)[0];
			break;
			case Action::SET_CONTENT_TERRAIN:
				std::cout << "SET TERRAIN ";
			break;
			default:
			break;
		}

	}
	if (*a.type == Action::TYPE_END) {
		switch (*a.code) {
			case Action::MOVE_UP:
				player->get<CInput>()->up = false;
			break;
			case Action::MOVE_LEFT:
				player->get<CInput>()->left = false;
			break;
			case Action::MOVE_DOWN:
				player->get<CInput>()->down = false;
			break;
			case Action::MOVE_RIGHT:
				player->get<CInput>()->right = false;
			break;
			default:
			break;
		}
	}
}

void SceneEditor::sView() {
	//update camera position
	cam.target = player->get<CTransform>()->pos;
	float square_delta = squareDistance(cam.pos, cam.target);

	if (square_delta > app_conf->cam_treshold) {
		cam.pos += ((cam.target - cam.pos) / app_conf->cam_speed);
	}

	//update view position
	int w = app_conf->modes[app_conf->current_mode_id].width;
	int h = app_conf->modes[app_conf->current_mode_id].height;
	sf::FloatRect world = level.map_ground.getBounds();
	sf::FloatRect rect(cam.pos.x-w/2, cam.pos.y-h/2, w, h);

	//fix weird lines between map tiles when moving
	rect.left = round(rect.left);
	rect.top = round(rect.top);

	if (rect.left < 0) rect.left = 0;
	if (rect.top < 0) rect.top = 0;
	if (rect.left + rect.width > world.width) rect.left = world.width - w;
	if (rect.top + rect.height > world.height) rect.top = world.height - h;

	game_view->reset(rect);
}
