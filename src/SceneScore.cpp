#include "SceneScore.h"
#include <cmath>
#include "SUpdate.h"
#include "SDraw.h"

SceneScore::SceneScore(Game* g)
	:Scene(g)
{
	init();
}
SceneScore::~SceneScore() {}

void SceneScore::init() {
	setFade(FADE_IN, 60);

	interface.add(game->assets->getWidget("score_menu"));

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

	{
		WidgetText* title = new WidgetText();
		std::string string = "STAGE 1";
		sf::Font& font = game->assets->getFont(Assets::FONT_COURIER);
		unsigned int size = static_cast<unsigned int>(game->app_conf.window_h * title_h);
		sf::Vector2i pos;
		pos.x = static_cast<int>(game->app_conf.window_w*0.5);
		pos.y = static_cast<int>(game->app_conf.window_h*(header_h+title_h/2));

		title->setText(string, font, size);
		title->setPosAbs(pos);
		interface.add(title);
	}

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
			else if (r == rows-2) {
				//string = "___";
			}

			// total score
			else if (r == rows-1) {
				if (c == 1) string = "TOTAL: ";
				if (c == 2) string = std::to_string(total_kills);
				if (c == 3) string = std::to_string(total_score);
			}

			// normal rows
			else {
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

			if (!string.empty()) {
				WidgetText* cell = new WidgetText();
				unsigned int size = static_cast<unsigned int>(game->app_conf.window_h*row_h * 0.8);
				sf::Vector2i pos;
				pos.x = static_cast<int>(game->app_conf.window_w * (c*col_w + indent_left + col_w/2));
				pos.y = static_cast<int>(game->app_conf.window_h * (r*row_h + header_h + title_h + spacer_h + row_h/2));

				cell->setText(string, font, size);
				cell->setColor(color);
				cell->setPosAbs(pos);
				interface.add(cell);
			}
		}
		if (r > 0) {
			it_k++;
		}
	}

	game->screen_tex.setView(gui_view);
}

void SceneScore::update() {
	SDraw::drawInterface(&game->screen_tex, interface.getWidgets());

	if (frame_current == 360) {
		setFade(FADE_OUT, 60, Game::GAME_SCENE_PLAY);
	}

	sFade();
	frame_current++;
}

void SceneScore::doAction(const Action* a) {
	if (*a->type == Action::TYPE_START) {
		switch (*a->code) {
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
