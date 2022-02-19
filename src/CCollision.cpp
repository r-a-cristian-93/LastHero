#include "CCollision.h"

CCollision::CCollision()
	:radius(0)
	,offset()
	{}

CCollision::CCollision(const CCollision& c)
	:radius(c.radius)
	,offset(c.offset)
	{}

CCollision::CCollision(int r)
	:radius(r)
	,offset()
	{}
