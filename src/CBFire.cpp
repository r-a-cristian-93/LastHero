#include "CBFire.h"

CBFire::CBFire()
	:target(nullptr)
	{}

CBFire::CBFire(const CBFire& c)
	:pri(c.pri)
	,sec(c.sec)
	,target(c.target)
	{}

CBFire::CBFire(std::vector<BCondition> _pri, std::vector<BCondition> _sec)
	:pri(_pri)
	,sec(_sec)
	,target(nullptr)
	{}
