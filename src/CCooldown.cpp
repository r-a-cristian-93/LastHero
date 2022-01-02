#include "CCooldown.h"

CCooldown::CCooldown()
	:cooldown(0)
	,remaining(0)
	{}
	
CCooldown::CCooldown(int c)
	:cooldown(c)
	,remaining(c)
	{}
