#include "CTransform.h"

CTransform::CTransform()
	:max_velocity(0)
	,pos({0,0})
	,dir({0,0})
	,vel({0,0})
	,d_angle(0)
	{}

CTransform::CTransform(const float& v)
	:max_velocity(v)
	,pos(0, 0)
	,dir({0,0})
	,vel({0,0})
	,d_angle(v)
	{}

CTransform::CTransform(const sf::Vector2f& p, const float& v)
	:max_velocity(v)
	,pos(p)
	,dir({0,0})
	,vel({0,0})
	,d_angle(v)
	{}

CTransform::CTransform(const sf::Vector2f& p, const sf::Vector2f& d, const float& v)
	:max_velocity(v)
	,pos(p)
	,dir(d)
	,vel({0,0})
	,d_angle(v)
	{}
