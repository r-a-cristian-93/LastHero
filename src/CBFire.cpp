#include "CBFire.h"

CBFire::CBFire()
	:pri(0, 0)
	,sec(0, 0)
	,target(nullptr)
	{}

CBFire::CBFire(const CBFire& c)
	:pri(c.pri)
	,sec(c.sec)
	,target(c.target)
	{}

CBFire::CBFire(size_t tfp, size_t tfs, size_t dfp, size_t dfs)
	:pri(tfp, dfp)
	,sec(tfs, dfs)
	,target(nullptr)
	{}
