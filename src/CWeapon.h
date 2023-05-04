#ifndef C_WEAPON
#define C_WEAPON

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class CWeapon {
public:
	// dir, dx, dy - offset for projectile spawning dependent on direction and relative to entity's position
	std::map<size_t, sf::Vector2f> projectile_spawn;

	size_t primary;
	size_t secondary;

	size_t p_tag;
	size_t s_tag;
	float p_cooldown;
	float s_cooldown;
	float p_cooldown_current;
	float s_cooldown_current;
	int p_rounds;
	int s_rounds;
	int p_rounds_current;
	int s_rounds_current;
	float p_delay;
	float s_delay;
	float p_delay_current;
	float s_delay_current;
	size_t p_sfx;
	size_t s_sfx;
	size_t p_range;
	size_t s_range;

	CWeapon();
	CWeapon(const CWeapon& c);
	CWeapon(size_t primary, size_t secondary);
};

#endif
