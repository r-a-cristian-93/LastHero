#ifndef C_COLLISION
#define C_COLLISION

class CCollision {
public:
	int radius;

	CCollision();
	CCollision(const CCollision& c);
	CCollision(int r);
};

#endif
