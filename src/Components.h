#ifndef COMPONENTS
#define COMPONENTS

#include "CTransform.h"
#include "CCollision.h"
#include "CShape.h"
#include "CInput.h"
#include "CLifespan.h"
#include "CScore.h"
#include "CTarget.h"
#include "CStats.h"
#include "CAnimation.h"
#include "CWeapon.h"
#include "CFx.h"
#include "CSfx.h"
#include "CBFire.h"
#include "CBPatrol.h"
#include "CBChase.h"
#include "CBPowerup.h"

class Components {
	std::tuple<
		CTransform*,
		CShape*,
		CCollision*,
		CInput*,
		CLifespan*,
		CScore*,
		CTarget*,
		CStats*,
		CAnimation*,
		CWeapon*,
		CFx*,
		CSfx*,
		CBFire*,
		CBPatrol*,
		CBChase*,
		CBPowerup*> components;

public:
	Components() {
		add<CTransform>(nullptr);
		add<CShape>(nullptr);
		add<CCollision>(nullptr);
		add<CInput>(nullptr);
		add<CLifespan>(nullptr);
		add<CScore>(nullptr);
		add<CTarget>(nullptr);
		add<CStats>(nullptr);
		add<CAnimation>(nullptr);
		add<CWeapon>(nullptr);
		add<CFx>(nullptr);
		add<CSfx>(nullptr);
		add<CBFire>(nullptr);
		add<CBPatrol>(nullptr);
		add<CBChase>(nullptr);
		add<CBPowerup>(nullptr);
	}

	Components(
		CTransform* c_tr,
		CShape* c_sh,
		CCollision* c_c,
		CInput* c_i,
		CLifespan* c_l,
		CScore* c_s,
		CTarget* c_tg,
		CStats* c_h,
		CWeapon* c_w,
		CFx* c_fx,
		CSfx* c_sfx,
		CAnimation* c_a,
		CBFire* cb_f,
		CBPatrol* cb_p,
		CBChase* cb_c,
		CBPowerup* cb_pw)
	{
		add<CTransform>(c_tr);
		add<CShape>(c_sh);
		add<CCollision>(c_c);
		add<CInput>(c_i);
		add<CLifespan>(c_l);
		add<CScore>(c_s);
		add<CTarget>(c_tg);
		add<CStats>(c_h);
		add<CWeapon>(c_w);
		add<CFx>(c_fx);
		add<CSfx>(c_sfx);
		add<CAnimation>(c_a);
		add<CBFire>(cb_f);
		add<CBPatrol>(cb_p);
		add<CBChase>(cb_c);
		add<CBPowerup>(cb_pw);
	}

	Components(const Components& c) {
		if (c.get<CTransform>())
			add<CTransform>(new CTransform(*c.get<CTransform>()));
		if (c.get<CShape>())
			add<CShape>(new CShape(*c.get<CShape>()));
		if (c.get<CCollision>())
			add<CCollision>(new CCollision(*c.get<CCollision>()));
		if (c.get<CInput>())
			add<CInput>(new CInput(*c.get<CInput>()));
		if (c.get<CLifespan>())
			add<CLifespan>(new CLifespan(*c.get<CLifespan>()));
		if (c.get<CScore>())
			add<CScore>(new CScore(*c.get<CScore>()));
		if (c.get<CTarget>())
			add<CTarget>(new CTarget(*c.get<CTarget>()));
		if (c.get<CStats>())
			add<CStats>(new CStats(*c.get<CStats>()));
		if (c.get<CAnimation>())
			add<CAnimation>(new CAnimation(*c.get<CAnimation>()));
		if (c.get<CWeapon>())
			add<CWeapon>(new CWeapon(*c.get<CWeapon>()));
		if (c.get<CFx>())
			add<CFx>(new CFx(*c.get<CFx>()));
		if (c.get<CSfx>())
			add<CSfx>(new CSfx(*c.get<CSfx>()));
		if (c.get<CBFire>())
			add<CBFire>(new CBFire(*c.get<CBFire>()));
		if (c.get<CBPatrol>())
			add<CBPatrol>(new CBPatrol(*c.get<CBPatrol>()));
		if (c.get<CBChase>())
			add<CBChase>(new CBChase(*c.get<CBChase>()));
		if (c.get<CBPowerup>())
			add<CBPowerup>(new CBPowerup(*c.get<CBPowerup>()));
	}

	~Components() {
		if (get<CTransform>()) delete get<CTransform>();
		if (get<CShape>()) delete get<CShape>();
		if (get<CCollision>()) delete get<CCollision>();
		if (get<CInput>()) delete get<CInput>();
		if (get<CLifespan>()) delete get<CLifespan>();
		if (get<CScore>()) delete get<CScore>();
		if (get<CTarget>()) delete get<CTarget>();
		if (get<CStats>()) delete get<CStats>();
		if (get<CAnimation>()) delete get<CAnimation>();
		if (get<CWeapon>()) delete get<CWeapon>();
		if (get<CFx>()) delete get<CFx>();
		if (get<CSfx>()) delete get<CSfx>();
		if (get<CBFire>()) delete get<CBFire>();
		if (get<CBPatrol>()) delete get<CBPatrol>();
		if (get<CBChase>()) delete get<CBChase>();
		if (get<CBPowerup>()) delete get<CBPowerup>();
	}

	template<class T>
	void add(T* t) {
		std::get<T*>(components) = t;
	}

	template<class T>
	T* get() const {
		return std::get<T*>(components);
	}
} ;

#endif
