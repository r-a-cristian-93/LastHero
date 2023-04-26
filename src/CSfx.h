#ifndef C_SFX
#define C_SFX

#include "Common.h"

class CSfx {
public:
	size_t hurt = 0;
	size_t die = 0;
	size_t spawn = 0;
	bool playing = false;

	CSfx();
	CSfx(const CSfx& c);
	CSfx(size_t hurt, size_t die, size_t spawn);
};

#endif
