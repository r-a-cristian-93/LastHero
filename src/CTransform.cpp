#include "CTransform.h"

CTransform::CTransform()
	:max_velocity(0)
	,pos({0,0})
	,dir({0,0})
	,prev_pos({0,0})
	,prev_dir({1,1})
	,vel({0,0})
	,d_angle(0)
	{}

CTransform::CTransform(const CTransform& c)
	:max_velocity(c.max_velocity)
	,pos(c.pos)
	,dir(c.dir)
	,prev_pos(c.prev_pos)
	,prev_dir(c.prev_dir)
	,vel(c.vel)
	,d_angle(c.d_angle)
	{}

CTransform::CTransform(const float& v)
	:max_velocity(v)
	,pos(0, 0)
	,dir({0,0})
	,prev_pos({0,0})
	,prev_dir({1,1})
	,vel({0,0})
	,d_angle(v)
	{}

CTransform::CTransform(const sf::Vector2f& p, const float& v)
	:max_velocity(v)
	,pos(p)
	,dir({0,0})
	,prev_pos({0,0})
	,prev_dir({1,1})
	,vel({0,0})
	,d_angle(v)
	{}

CTransform::CTransform(const sf::Vector2f& p, const sf::Vector2f& d, const float& v)
	:max_velocity(v)
	,pos(p)
	,dir(d)
	,prev_pos(p)
	,prev_dir(d)
	,vel({0,0})
	,d_angle(v)
	{}
