#include "SceneScore.h"
#include <cmath>
#include "SUpdate.h"
#include "SDraw.h"

SceneScore::SceneScore(Game* g)
	:Scene(g)
{
	init();
}

void SceneScore::init() {
	setFade(FADE_IN, 60);

	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::N, Action::MENU_SELECT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::M, Action::MENU_SELECT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Enter, Action::MENU_SELECT);
	game->act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Escape, Action::MENU_SELECT);

	{
		title = new WidgetText();
		std::string string = "";
		if (game->stagePrev() + 1 == game->stagesCount()) {
			string = "ALL STAGES COMPLETE!";
		}
		else {
			string = "STAGE " + std::to_string(game->stagePrev() + 1) + " COMPLETE";
		}

		sf::Font& font = game->assets->getFont(Assets::FONT_COURIER);
		unsigned int size = static_cast<unsigned int>(game->app_conf.window_h * title_h);
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.window_w*0.5);
		pos.y = static_cast<int>(game->app_conf.window_h*(header_h+title_h/2));

		title->setText(string, font, size);
		title->setPosAbs(pos);
		interface.add(title);
	}

	const KillsMap& kills = game->kills_per_enemy;
	const KillsMap& n_kills = game->new_kills_per_enemy;
	KillsMap::const_iterator it_k;

	for (it_k = kills.cbegin(); it_k != kills.cend(); it_k++) {
		// calculate total_kills and total_score
		total_kills += it_k->second;

		// calculate total_score
		size_t points = it_k->second * game->assets->getScorePoints(it_k->first);
		total_score += points;

		// count rows
		rows++;
	}

	// calculate row_h
	row_h = (1 - header_h - title_h - spacer_h - footer_h) / rows;
	if (row_h > title_h*0.6) row_h = title_h*0.6;

	//calculate col_w
	col_w = (1 - indent_left - indent_right) / cols;

	std::string string = "";
	sf::Font& font = game->assets->getFont(Assets::FONT_COURIER);
	sf::Color color({255,255,255});

	it_k = kills.cbegin();
	for (int r=0; r<rows; r++) {
		for (int c=0; c<cols; c++) {
			string = "";
			color = {255, 255, 255};

			// table header
			if (r == 0) {
				color = {255, 100, 100};

				if (c == 2) string = "KILLS";
				else if (c == 3) string = "POINTS";
			}

			// line
			else if (r == rows-2 && c > 1) {
				string = "____";
			}

			// total score
			else if (r == rows-1) {
				if (c == 1) string = "TOTAL: ";
				if (c == 2) string = std::to_string(total_kills);
				if (c == 3) string = std::to_string(total_score);
			}

			// normal rows
			else if (!kills.empty()) {
				if (c == 0)
					string = "ICO " + std::to_string(it_k->first); // ENTITY ICON;
				if (c == 1)
					if (n_kills.count(it_k->first))
						string = "+" + std::to_string(n_kills.at(it_k->first));
				if (c == 2)
					string = std::to_string(it_k->second);
				if (c == 3) {
					size_t points = it_k->second * game->assets->getScorePoints(it_k->first);
					string = std::to_string(points);
				}
			}


			WidgetText* cell = new WidgetText();
			unsigned int size = static_cast<unsigned int>(game->app_conf.window_h*row_h * 0.8);
			sf::Vector2i pos;
			pos.x = static_cast<int>(game->app_conf.window_w * (c*col_w + indent_left + col_w/2));
			pos.y = static_cast<int>(game->app_conf.window_h * (r*row_h + header_h + title_h + spacer_h + row_h/2));

			cell->setText(string, font, size);
			cell->setColor(color);
			cell->setPosAbs(pos);
			all_table_widgets.push_back(cell);

		}
		if (r > 0) {
			if (!kills.empty()) it_k++;
		}
	}

	game->screen_tex.setView(gui_view);
}

void SceneScore::update() {
	if (frame_current == FRAME_COL_0) copyCells(all_table_widgets, table_widgets, {0,0,0,rows-3});
	if (frame_current == FRAME_COL_1) copyCells(all_table_widgets, table_widgets, {1,0,1,rows-3});
	if (frame_current == FRAME_COL_2) copyCells(all_table_widgets, table_widgets, {2,0,2,rows-3});
	if (frame_current == FRAME_COL_3) copyCells(all_table_widgets, table_widgets, {3,0,3,rows-3});
	if (frame_current == FRAME_ROW_LINE) copyCells(all_table_widgets, table_widgets, {2,rows-2,3,rows-2});
	if (frame_current == FRAME_ROW_TOTAL) copyCells(all_table_widgets, table_widgets, {1,rows-1,3,rows-1});

	SDraw::drawInterface(&game->screen_tex, interface.getWidgets());
	SDraw::drawInterface(&game->screen_tex, table_widgets);

	sFade();
	frame_current++;
}

void SceneScore::copyCells(WidgetVec& src, WidgetVec& dst, sf::IntRect rect) {
	int k = 0;
	for (int r=0; r<rows; r++) {
		for (int c=0; c<cols; c++) {
			if (r >= rect.top && r <= rect.height && c >= rect.left && c <= rect.width) {
				dst.push_back(src[k]);
			}
			k++;
		}
	}
}

void SceneScore::doAction(const Action* a) {
	if (*a->type == Action::TYPE_START) {
		switch (*a->code) {
			case Action::MENU_SELECT:
				if (frame_current > FRAME_CONTINUE) {
					if (game->stageCurrent()) {
						setFade(FADE_OUT, 60, Game::GAME_SCENE_PLAY);
					}
					else {
						setFade(FADE_OUT, 60, Game::GAME_SCENE_MENU);
					}
				}
			break;
			default:
			break;
		}
	}
	if (*a->type == Action::TYPE_END) {
		switch (*a->code) {
			default:
			break;
		}
	}
}

SceneScore::~SceneScore() {
	for (Widget* w : all_table_widgets) {
		delete w;
	}

	delete title;
}
