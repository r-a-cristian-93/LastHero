#include "SceneMainMenu.h"
#include <cmath>
#include "SDraw.h"

SceneMainMenu::SceneMainMenu()
	:Scene(GAME_SCENE::MENU)
	,background(nullptr)
{
	init();
}
SceneMainMenu::~SceneMainMenu() {}

void SceneMainMenu::init() {
	music_fade_out = true;
	//snd_mgr->playBgMusic("intro");

	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::N, Action::MENU_SELECT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::M, Action::MENU_SELECT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Enter, Action::MENU_SELECT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Escape, Action::GAME_EXIT);

	// set and scale background
	background = &assets->getSprite("main_bg");
	sf::FloatRect b = background->getLocalBounds();
	float scale_x = app_conf->game_w / b.width;
	float scale_y = app_conf->game_h / b.height;
	background->setScale(scale_x, scale_y);

	// set main menu buttons
	{
		Widget& play = assets->getWidget("button_play");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.5);
		pos.y = static_cast<int>(app_conf->game_h*0.7);
		play.setColor(mod_highlight);
		play.setPosAbs(pos);
		interface.add(play);
	}

	{
		Widget& settings = assets->getWidget("button_settings");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.5);
		pos.y = static_cast<int>(app_conf->game_h*0.77);
		settings.setColor(mod_dark);
		settings.setPosAbs(pos);
		interface.add(settings);
	}

	{
		Widget& exit = assets->getWidget("button_exit");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.5);
		pos.y = static_cast<int>(app_conf->game_h*0.84);
		exit.setColor(mod_dark);
		exit.setPosAbs(pos);
		interface.add(exit);
	}

	screen_tex->setView(gui_view);
}

void SceneMainMenu::update() {
	interface.update();

	screen_tex->draw(*background);
	SDraw::drawInterface(&*screen_tex, interface.getWidgets());

	frame_current++;
}

void SceneMainMenu::doAction(const Action& a) {
	if (*a.type == Action::TYPE_START) {
		switch (*a.code) {
			case Action::MENU_SELECT:
				if (getCurrentFade() != FADE::OUT) {
					snd_mgr->playSound("menu_confirm");
					if (selection == SELECT_EXIT) {
						setFade(FADE::OUT, GAME_SCENE::EXIT);
					}
					else if (selection == SELECT_PLAY) {
						setFade(FADE::OUT, GAME_SCENE::PLAY);
					}
					else if (selection == SELECT_SETTINGS) {
						setFade(FADE::OUT, GAME_SCENE::SETTINGS);
					}
				}
			break;
			case Action::MOVE_UP:
				if (getCurrentFade() != FADE::OUT) {
					if (selection > 0) select(selection-1);
				}
			break;
			case Action::MOVE_DOWN:
				if (getCurrentFade() != FADE::OUT) {
					if (selection < SELECT_COUNT-1) {
						select(selection + 1);
					}
				}
			break;
			case Action::GAME_EXIT:
				snd_mgr->playSound("menu_cancel");
				setFade(FADE::OUT, GAME_SCENE::EXIT);
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

void SceneMainMenu::select(size_t s) {
	selection = s;

	for (size_t i = 0; i<interface.getWidgets().size(); i++) {
		if (i == selection) {
			interface.getWidgets()[i].setColor(mod_highlight);
		}
		else {
			interface.getWidgets()[i].setColor(mod_dark);
		}
	}

	snd_mgr->playSound("menu_select");
}
