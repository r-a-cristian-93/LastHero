#ifndef C_LIFESPAN
#define C_LIFESPAN

class CLifespan {
public:
	float lifespan;
	float remaining;

	CLifespan();
	CLifespan(const CLifespan& c);
	CLifespan(float l);
};

#endif
