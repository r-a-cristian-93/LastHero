#include "CSfx.h"

CSfx::CSfx()
	{}

CSfx::CSfx(const CSfx& c)
	:hurt(c.hurt)
	,die(c.die)
	,spawn(c.spawn)
	{}

CSfx::CSfx(size_t _hurt, size_t _die, size_t _spawn)
	:hurt(_hurt)
	,die(_die)
	,spawn(_spawn)
	{}
