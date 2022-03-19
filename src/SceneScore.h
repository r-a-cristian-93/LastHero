#ifndef SCENE_GAME_SCORE
#define SCENE_GAME_SCORE

#include "Scene.h"
#include "Interface.h"

typedef std::vector<Widget*> ScoreRow;
typedef std::vector<ScoreRow> ScoreTable;

/*
 * The scoreboard is spaced vertically as follows
 *
 * HEADER
 * TITLE
 * SPACER
 * ROW
 * ROw
 * ...
 * ROW
 * ROW
 * LINE
 * TOTAL_ROW
 * FOOTER
 */

class SceneScore: public Scene {
	Interface interface;

	ScoreTable table;
	sf::RectangleShape line;
	ScoreRow total_row;
	unsigned int total_kills = 0;
	unsigned int total_score = 0;

	size_t rows = 3; // table header + line + total row;
	float title_h = 0.10; // 10% of screen height
	float header_h = 0.05;
	float footer_h = 0.05;
	float spacer_h = 0.05;
	float row_h = 0;

	size_t cols = 4;
	float indent_left = 0.1;
	float indent_right = 0.1;
	float col_w = 0;

	enum {
		SELECT_PLAY,
		SELECT_EXIT,
	};

	size_t selection = SELECT_PLAY;

	void init();

public:
	void update() override;
	void doAction(const Action* a) override;

	SceneScore();
	SceneScore(Game* g);
	~SceneScore();
};

#endif
