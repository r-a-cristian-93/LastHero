#include "CLifespan.h"

CLifespan::CLifespan()
	:lifespan(0)
	,remaining(0)
	{}
	
CLifespan::CLifespan(int l)
	:lifespan(l)
	,remaining(l)
	{}
