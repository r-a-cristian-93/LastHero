#ifndef COMPONENTS
#define COMPONENTS

#include "CTransform.h"
#include "CCollision.h"
#include "CShape.h"
#include "CInput.h"
#include "CLifespan.h"
#include "CScore.h"
#include "CTarget.h"

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
