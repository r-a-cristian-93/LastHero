#include "CBChase.h"

CBChase::CBChase()
	:cond()
	{}

CBChase::CBChase(const CBChase& c)
	:cond(c.cond)
	{}

CBChase::CBChase(std::vector<BCondition>& _cond)
	:cond(_cond)
	{}
