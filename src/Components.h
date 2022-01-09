#ifndef COMPONENTS
#define COMPONENTS

#include "CTransform.h"
#include "CCollision.h"
#include "CShape.h"
#include "CInput.h"
#include "CLifespan.h"
#include "CScore.h"
#include "CTarget.h"
#include <iostream>

class Components {
	std::tuple<
		CTransform*,
		CShape*,
		CCollision*,
		CInput*,
		CLifespan*,
		CScore*,
		CTarget*> components;

public:
	Components() {
		add<CTransform>(nullptr);
		add<CShape>(nullptr);
		add<CCollision>(nullptr);
		add<CInput>(nullptr);
		add<CLifespan>(nullptr);
		add<CScore>(nullptr);
		add<CTarget>(nullptr);
	}

	Components(
		CTransform* c_tr,
		CShape* c_sh,
		CCollision* c_c,
		CInput* c_i,
		CLifespan* c_l,
		CScore* c_s,
		CTarget* c_tg)
	{
		add<CTransform>(c_tr);
		add<CShape>(c_sh);
		add<CCollision>(c_c);
		add<CInput>(c_i);
		add<CLifespan>(c_l);
		add<CScore>(c_s);
		add<CTarget>(c_tg);
	}

	Components(Components& c) {
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
	}

	~Components() {
		if (get<CTransform>()) delete get<CTransform>();
		if (get<CShape>()) delete get<CShape>();
		if (get<CCollision>()) delete get<CCollision>();
		if (get<CInput>()) delete get<CInput>();
		if (get<CLifespan>()) delete get<CLifespan>();
		if (get<CScore>()) delete get<CScore>();
		if (get<CTarget>()) delete get<CTarget>();
	}

	template<class T>
	void add(T* t) {
		std::get<T*>(components) = t;
	}

	template<class T>
	T* get() {
		return std::get<T*>(components);
	}
} ;

#endif
