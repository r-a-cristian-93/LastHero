#include "CFx.h"

CFx::CFx()
	:fx()
	{}

CFx::CFx(const CFx& c)
	:fx(c.fx)
	{}

CFx::CFx(std::vector<Fx> _fx)
	:fx(_fx)
	{}
