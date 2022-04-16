#include "CStats.h"
#include <iostream>

CStats::CStats()
	:experience(0)
	,level(0)
	,base{0}
	,per_level{0}
	,initial{0}
	,effective{0}
	{}

CStats::CStats(const CStats& c)
	:experience(c.experience)
	,level(c.level)

{
	for (int i=0; i<COUNT; i++) {
		base[i] = c.base[i];
		per_level[i] = c.per_level[i];
		initial[i] = c.initial[i];
		effective[i] = c.effective[i];
	}
}
