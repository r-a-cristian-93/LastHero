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

#endif
