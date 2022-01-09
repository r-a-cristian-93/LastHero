#ifndef C_LIFESPAN
#define C_LIFESPAN

class CLifespan {
public:
	int lifespan;
	int remaining;
	
	CLifespan();
	CLifespan(const CLifespan& c);
	CLifespan(int l);
};

#endif
