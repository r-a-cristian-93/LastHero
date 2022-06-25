#ifndef ENUMS
#define ENUMS

namespace TAG {
	enum {
		NONE = 0,
		PLAYER,
		BASE,
		ENEMY,
		ENVIRONMENT,
		PROJECTILE,
		MISSLE,
		FX,
	};
}

namespace TR {
	enum {
		NONE = 0,
		PLAYER_LOW_HP,			// % of HP
		PLAYER_NEARBY,			// distance in pixels
		BASE_LOW_HP,				// % of HP
		BASE_NEARBY,				// distance in pixels
		BASE_NOT_PROTECTED,		// distance in pixels
		CONTINUOUS,
		RANDOM,
		SPAWN,
		DIE,
	};
}


namespace GAME_SCENE {
	enum {
		NONE = 0,
		MENU,
		SETTINGS,
		PLAY,
		OVER,
		SCORE,
		EXIT,
		COUNT
	};
}

namespace FADE {
	enum {
		NONE = 0,
		IN,
		OUT,
		COUNT
	};
}

namespace FRAMES_SCORE {
	enum {
		NONE = 0,
		COL_0,
		COL_1,
		COL_2,
		COL_3,
		ROW_LINE,
		ROW_TOTAL,
		CONTINUE,
		COUNT
	};
}

#endif
