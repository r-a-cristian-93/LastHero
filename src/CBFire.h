#ifndef C_B_FIRE
#define C_B_FIRE

#include <string>

class CBFire {
public:
	enum {
		NONE = 0,
		TR_PLAYER_LOW_HP,
		TR_PLAYER_NEARBY,
		TR_BASE_LOW_HP,
		TR_BASE_NEARBY,
		TR_BASE_NOT_PROTECTED,
		TR_CONTINUOUS,
		TR_RANDOM,
	};

	size_t tr_fire_pri;
	size_t tr_fire_sec;
	size_t data_fire_pri;	//can be distance or HP;
	size_t data_fire_sec;

	CBFire();
	CBFire(const CBFire& c);
	CBFire(size_t tr_fire_pri, size_t tr_fire_sec, size_t data_fire_pri, size_t data_fire_sec);
};

#endif
