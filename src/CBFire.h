#ifndef C_B_FIRE
#define C_B_FIRE

#include <memory>
#include "CommonBehaviour.h"

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
	BFire pri;
	BFire sec;

	std::shared_ptr<Entity> target;

	CBFire();
	CBFire(const CBFire& c);
	CBFire(size_t tr_fire_pri, size_t tr_fire_sec, size_t data_fire_pri, size_t data_fire_sec);
};

#endif
