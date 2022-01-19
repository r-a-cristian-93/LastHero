#ifndef C_STATS
#define C_STATS

class CStats {
public:
	enum {
		HEALTH,
		DEFENCE,
		ATTACK,
		COUNT
	};

	int level, experience;

	int base[COUNT];		// level 1 values
	int per_level[COUNT];	// increase per level values
	int initial[COUNT];		// base + level*per_level + gear
	int effective[COUNT];	// after taking damage
	// effective shield decreases when taking damage
	// effective health decreases when shield is 0 and taking damage
	// effective attack does not change

	CStats();
	CStats(const CStats& c);
};

#endif
