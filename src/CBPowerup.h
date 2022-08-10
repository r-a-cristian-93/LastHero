#ifndef C_B_POWERUP
#define C_B_POWERUP

#include <memory>
#include <vector>
#include "CommonBehaviour.h"

class Entity;

class CBPowerup {
public:
	enum {
		NONE = 0,
		PLAYER_HP,
		BASE_HP,
		WEAPON_ROUNDS
	};

	BCondition cond;
	size_t powerup;
	size_t percent; // percent of base value

	CBPowerup();
	CBPowerup(const CBPowerup& c);
	CBPowerup(BCondition cond, size_t powerup, size_t percent);
};

#endif
