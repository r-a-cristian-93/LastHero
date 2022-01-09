#include "CCollision.h"

CCollision::CCollision()
	:radius(0)
	{}

CCollision::CCollision(const CCollision& c)
	:radius(c.radius)
	{}

CCollision::CCollision(int r)
	:radius(r)
	{}
