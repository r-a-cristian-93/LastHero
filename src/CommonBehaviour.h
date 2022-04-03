#ifndef COMMON_BEHAVIOUR
#define COMMON_BEHAVIOUR

enum {
	NONE = 0,
	TR_PLAYER_LOW_HP,			// % of HP
	TR_PLAYER_NEARBY,			// distance in pixels
	TR_BASE_LOW_HP,				// % of HP
	TR_BASE_NEARBY,				// distance in pixels
	TR_BASE_NOT_PROTECTED,		// distance in pixels
	TR_CONTINUOUS,
	TR_RANDOM,
};


struct BCondition {
	size_t trigger = 0;
	size_t data_start = 0;
	size_t data_stop = 0;
};

#endif
