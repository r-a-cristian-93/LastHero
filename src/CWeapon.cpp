#include "CWeapon.h"

CWeapon::CWeapon()
	:primary("")
	,secondary("")
	,p_tag(0)
	,s_tag(0)
	,p_cooldown(0)
	,s_cooldown(0)
	,p_cooldown_current(0)
	,s_cooldown_current(0)
	,projectile_spawn()
	{}

CWeapon::CWeapon(const CWeapon& c)
	:primary(c.primary)
	,secondary(c.secondary)
	,p_tag(c.p_tag)
	,s_tag(c.s_tag)
	,p_cooldown(c.p_cooldown)
	,s_cooldown(c.s_cooldown)
	,p_cooldown_current(c.p_cooldown_current)
	,s_cooldown_current(c.s_cooldown_current)
	,projectile_spawn(c.projectile_spawn)
	{}

CWeapon::CWeapon(std::string& _primary, std::string& _secondary)
	:primary(_primary)
	,secondary(_secondary)
	,p_tag(0)
	,s_tag(0)
	,p_cooldown(0)
	,s_cooldown(0)
	,p_cooldown_current(0)
	,s_cooldown_current(0)

	,projectile_spawn()
	{}
