#ifndef C_COOLDOWN
#define C_COOLDOWN

class CCooldown {
public:
	int cooldown;
	int remaining;
	
	CCooldown();
	CCooldown(int c);
};

#endif
