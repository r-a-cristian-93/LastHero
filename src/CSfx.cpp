#include "CSfx.h"

CSfx::CSfx()
	{}

CSfx::CSfx(const CSfx& c)
	:hurt(c.hurt)
	,die(c.die)
	{}

CSfx::CSfx(size_t _hurt, size_t _die)
	:hurt(_hurt)
	,die(_die)
	{}
