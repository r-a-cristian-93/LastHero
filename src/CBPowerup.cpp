#include "CBPowerup.h"

CBPowerup::CBPowerup()
	:cond()
	,powerup(0)
	,percent(0)
	{}

CBPowerup::CBPowerup(const CBPowerup& c)
	:cond(c.cond)
	,powerup(c.powerup)
	,percent(c.percent)
	{}

CBPowerup::CBPowerup(BCondition _cond, size_t _powerup, size_t _percent)
	:cond(_cond)
	,powerup(_powerup)
	,percent(_percent)
	{}
