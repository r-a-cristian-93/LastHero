#include "CWeapon.h"

CWeapon::CWeapon()
	:primary(0)
	,secondary(0)
	,p_tag(0)
	,s_tag(0)
	,p_cooldown(0)
	,s_cooldown(0)
	,p_cooldown_current(0)
	,s_cooldown_current(0)
	,p_rounds(0)
	,s_rounds(0)
	,p_delay(0)
	,s_delay(0)
	,p_delay_current(0)
	,s_delay_current(0)
	,p_sfx(0)
	,s_sfx(0)
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
	,p_rounds(c.p_rounds)
	,s_rounds(c.s_rounds)
	,p_delay(c.p_delay)
	,s_delay(c.s_delay)
	,p_delay_current(c.p_delay)
	,s_delay_current(c.s_delay)
	,p_sfx(c.p_sfx)
	,s_sfx(c.s_sfx)
	,projectile_spawn(c.projectile_spawn)
	{}

CWeapon::CWeapon(size_t _primary, size_t _secondary)
	:primary(_primary)
	,secondary(_secondary)
	,p_tag(0)
	,s_tag(0)
	,p_cooldown(0)
	,s_cooldown(0)
	,p_cooldown_current(0)
	,s_cooldown_current(0)
	,p_rounds(0)
	,s_rounds(0)
	,p_delay(0)
	,s_delay(0)
	,p_delay_current(0)
	,s_delay_current(0)
	,p_sfx(0)
	,s_sfx(0)
	,projectile_spawn()
	{}
