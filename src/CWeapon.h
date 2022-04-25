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
	size_t p_cooldown;
	size_t s_cooldown;
	size_t p_cooldown_current;
	size_t s_cooldown_current;
	size_t p_rounds;
	size_t s_rounds;
	size_t p_delay;
	size_t s_delay;
	size_t p_delay_current;
	size_t s_delay_current;
	size_t p_sfx;
	size_t s_sfx;

	CWeapon();
	CWeapon(const CWeapon& c);
	CWeapon(size_t primary, size_t secondary);
};

#endif
