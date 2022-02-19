#include "CWeapon.h"

CWeapon::CWeapon()
	:primary("")
	,secondary("")
	,projectile_spawn()
	{}

CWeapon::CWeapon(const CWeapon& c)
	:primary(c.primary)
	,secondary(c.secondary)
	,projectile_spawn(c.projectile_spawn)
	{}

CWeapon::CWeapon(std::string& _primary, std::string& _secondary)
	:primary(_primary)
	,secondary(_secondary)
	{}
