#ifndef C_FX
#define C_FX

#include <vector>
#include "Common.h"
#include "Enums.h"

struct Fx {
	const size_t tag = TAG::FX;
	size_t id = 0;
	size_t trigger = 0;
};

class CFx {
public:
	std::vector<Fx> fx;

	CFx();
	CFx(const CFx& c);
	CFx(std::vector<Fx> fx);
};

#endif
