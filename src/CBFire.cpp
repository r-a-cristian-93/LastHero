#include "CBFire.h"

CBFire::CBFire()
	:tr_fire_pri(0)
	,tr_fire_sec(0)
	,data_fire_pri(0)
	,data_fire_sec(0)
	{}

CBFire::CBFire(const CBFire& c)
	:tr_fire_pri(c.tr_fire_pri)
	,tr_fire_sec(c.tr_fire_sec)
	,data_fire_pri(c.data_fire_pri)
	,data_fire_sec(c.data_fire_sec)
	{}

CBFire::CBFire(size_t tfp, size_t tfs, size_t dfp, size_t dfs)
	:tr_fire_pri(tfp)
	,tr_fire_sec(tfs)
	,data_fire_pri(dfp)
	,data_fire_sec(dfs)
	{}
