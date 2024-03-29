#include "SharedResources.h"
#include <cmath>
#include "SceneScore.h"

SceneScore::SceneScore()
	:Scene(GAME_SCENE::SCORE)
{
	init();
}

void SceneScore::init() {
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::N, Action::MENU_SELECT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::M, Action::MENU_SELECT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Enter, Action::MENU_SELECT);
	act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Escape, Action::MENU_SKIP);

	{
		Widget title;
		std::string string = "";
		if (game_stats->prev_stage + 1 == game_stats->stagesCount()) {
			string = "ALL STAGES COMPLETE!";
		}
		else {
			string = "STAGE " + std::to_string(game_stats->prev_stage + 1);
		}

		sf::Font& font = assets->getFont(Assets::FONT_PIXEL);
		unsigned int size = static_cast<unsigned int>(app_conf->game_h * title_h * 0.5);
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.5);
		pos.y = static_cast<int>(app_conf->game_h*(header_h+title_h/2));

		title.setText(string, font, size);
		title.setPosAbs(pos);
		interface.add(title);
	}

	{
		Widget& skip = assets->getWidget("menu_skip");
		sf::Vector2i pos;
		pos.x = static_cast<int>(app_conf->game_w*0.2);
		pos.y = static_cast<int>(app_conf->game_h*0.95);
		skip.setPosAbs(pos);
		interface.add(skip);
	}

	const KillsMap& kills = game_stats->kills_per_enemy;
	const KillsMap& n_kills = game_stats->new_kills_per_enemy;
	KillsMap::const_iterator it_k;

	for (it_k = kills.cbegin(); it_k != kills.cend(); it_k++) {
		// calculate total_kills and total_score
		total_kills += it_k->second;

		// calculate total_score
		size_t points = it_k->second * assets->getScorePoints(it_k->first);
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
	sf::Font& font = assets->getFont(Assets::FONT_PIXEL);
	sf::Color color({255,255,255});

	it_k = kills.cbegin();
	for (int r=0; r<rows; r++) {
		for (int c=0; c<cols; c++) {
			string = "";
			color = {255, 255, 255};
			unsigned int size = static_cast<unsigned int>(app_conf->game_h*row_h * 0.5);
			sf::Vector2i pos;
			pos.x = static_cast<int>(app_conf->game_w * (c*col_w + indent_left + col_w/2));
			pos.y = static_cast<int>(app_conf->game_h * (r*row_h + header_h + title_h + spacer_h + row_h/2));

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
				if (c == 1)
					if (n_kills.count(it_k->first))
						string = "+" + std::to_string(n_kills.at(it_k->first));
				if (c == 2)
					string = std::to_string(it_k->second);
				if (c == 3) {
					size_t points = it_k->second * assets->getScorePoints(it_k->first);
					string = std::to_string(points);
				}
			}

			if (c==0 && r > 0 && r < rows-2) {
				Widget& widget = assets->getEntityIcon(it_k->first);

				if (widget.get<WCImage>()) {
					sf::FloatRect rect = widget.get<WCImage>()->getLocalBounds();
					sf::Vector2f offset;

					if (rect.width) offset.x = -rect.width/2;
					if (rect.height) offset.y = -rect.height/2;

					widget.get<WCImage>()->setPosition(pos.x + offset.x, pos.y + offset.y);
				}

				all_table_widgets.push_back(widget);
			}
			else {
				Widget cell;
				cell.setText(string, font, size);
				cell.setColor(color);
				cell.setPosAbs(pos);
				all_table_widgets.push_back(cell);
			}
		}
		if (r > 0 && r < rows-3) {
			if (!kills.empty()) it_k++;
		}
	}

	screen_tex->setView(gui_view);
}

void SceneScore::update() {
	if ((frame_current >= key_frames[FRAMES_SCORE::COL_0] && !key_frames_rendered[FRAMES_SCORE::COL_0]) || skip_key_frames) {
		key_frames_rendered[FRAMES_SCORE::COL_0] = true;
		copyCells(all_table_widgets, interface.getWidgets(), {0,0,0,rows-3});
		if (total_kills && !skip_key_frames) snd_mgr->playSound("menu_punch");
	}
	if ((frame_current >= key_frames[FRAMES_SCORE::COL_1] && !key_frames_rendered[FRAMES_SCORE::COL_1]) || skip_key_frames) {
		key_frames_rendered[FRAMES_SCORE::COL_1] = true;
		copyCells(all_table_widgets, interface.getWidgets(), {1,0,1,rows-3});
		if (!game_stats->new_kills_per_enemy.empty() && !skip_key_frames) snd_mgr->playSound("menu_punch");
	}
	if ((frame_current >= key_frames[FRAMES_SCORE::COL_2] && !key_frames_rendered[FRAMES_SCORE::COL_2]) || skip_key_frames) {
		key_frames_rendered[FRAMES_SCORE::COL_2] = true;
		copyCells(all_table_widgets, interface.getWidgets(), {2,0,2,rows-3});
		if (total_kills && !skip_key_frames) snd_mgr->playSound("menu_punch");

	}if ((frame_current >= key_frames[FRAMES_SCORE::COL_3] && !key_frames_rendered[FRAMES_SCORE::COL_3]) || skip_key_frames) {
		key_frames_rendered[FRAMES_SCORE::COL_3] = true;
		copyCells(all_table_widgets, interface.getWidgets(), {3,0,3,rows-3});
		if (total_kills && !skip_key_frames) snd_mgr->playSound("menu_punch");
	}
	if ((frame_current >= key_frames[FRAMES_SCORE::ROW_LINE] && !key_frames_rendered[FRAMES_SCORE::ROW_LINE]) || skip_key_frames) {
		key_frames_rendered[FRAMES_SCORE::ROW_LINE] = true;
		copyCells(all_table_widgets, interface.getWidgets(), {2,rows-2,3,rows-2});
		if (total_kills && !skip_key_frames) snd_mgr->playSound("menu_punch");
	}
	if ((frame_current >= key_frames[FRAMES_SCORE::ROW_TOTAL] && !key_frames_rendered[FRAMES_SCORE::ROW_TOTAL]) || skip_key_frames) {
		key_frames_rendered[FRAMES_SCORE::ROW_TOTAL] = true;
		copyCells(all_table_widgets, interface.getWidgets(), {1,rows-1,3,rows-1});
		if (!skip_key_frames) snd_mgr->playSound("menu_punch");
	}
	screen_tex->clear(sf::Color(10, 70, 10));
	sDrawInterface();

	frame_current += frame_mgr->getIncrement();
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

void SceneScore::doAction(const Action& a) {
	if (*a.type == Action::TYPE_START) {
		switch (*a.code) {
			case Action::MENU_SKIP: {
				skip_key_frames = true;
				act_mgr->registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::Escape, Action::MENU_SELECT);
			}
			break;
			case Action::MENU_SELECT:
				if ((frame_current > key_frames[FRAMES_SCORE::CONTINUE] || skip_key_frames) && getCurrentFade()!=FADE::OUT) {
					if (game_stats->next_stage) {
						setFade(FADE::OUT, GAME_SCENE::PLAY);
					}
					else {
						setFade(FADE::OUT, GAME_SCENE::MENU);
					}
				}
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

SceneScore::~SceneScore() {}
