#include "CLifespan.h"

CLifespan::CLifespan()
	:lifespan(0)
	,remaining(0)
	{}

CLifespan::CLifespan(const CLifespan& c)
	:lifespan(c.lifespan)
	,remaining(c.remaining)
	{}

CLifespan::CLifespan(float l)
	:lifespan(l)
	,remaining(l)
	{}
