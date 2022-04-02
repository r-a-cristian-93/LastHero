#ifndef C_B_FIRE
#define C_B_FIRE

#include <memory>

class Entity;

struct BFire {
	size_t trigger = 0;
	size_t data = 0;

	BFire() {};
	BFire(size_t _trigger, size_t _data)
		:trigger(_trigger)
		,data(_data)
	{}
};

class CBFire {
public:
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

	BFire pri;
	BFire sec;

	std::shared_ptr<Entity> target;

	CBFire();
	CBFire(const CBFire& c);
	CBFire(size_t tr_fire_pri, size_t tr_fire_sec, size_t data_fire_pri, size_t data_fire_sec);
};

#endif
