#ifndef SCENE_GAME_SCORE
#define SCENE_GAME_SCORE

#include "Scene.h"

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
	enum {
		FRAME_COL_0 = 60,
		FRAME_COL_1 = 90,
		FRAME_COL_2 = 120,
		FRAME_COL_3 = 150,
		FRAME_ROW_LINE = 180,
		FRAME_ROW_TOTAL = 240,
		FRAME_CONTINUE = 280
	};
	const float* key_frames = app_conf->score_key_frames;
	bool key_frames_rendered[FRAMES_SCORE::COUNT] = {};

	WidgetVec all_table_widgets;
	WidgetVec table_widgets;

	unsigned int total_kills = 0;
	unsigned int total_score = 0;

	int rows = 3; // table header + line + total row;
	float title_h = 0.10; // 10% of screen height
	float header_h = 0.05;
	float footer_h = 0.05;
	float spacer_h = 0.05;
	float row_h = 0;

	int cols = 4;
	float indent_left = 0.1;
	float indent_right = 0.1;
	float col_w = 0;

	enum {
		SELECT_PLAY,
		SELECT_EXIT,
	};

	size_t selection = SELECT_PLAY;

	void init();
	void copyCells(WidgetVec& src, WidgetVec& dst, sf::IntRect rec);

public:
	void update() override;
	void doAction(const Action& a) override;

	SceneScore();
	~SceneScore();
};

#endif
