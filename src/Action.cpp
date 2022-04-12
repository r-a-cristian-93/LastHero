#include "Action.h"

Action::Action()
	:code(nullptr)
	,type(nullptr)
	,ent_tag(nullptr)
	,ent_name(nullptr)
	,state(nullptr)
	,facing(nullptr)
	,pos(nullptr)
	,dir(nullptr)
	{}

Action::Action(size_t c, size_t t)
	:code(new size_t(c))
	,type(new size_t(t))
	,ent_tag(nullptr)
	,ent_name(nullptr)
	,state(nullptr)
	,facing(nullptr)
	,pos(nullptr)
	,dir(nullptr)
	{}

Action::Action(size_t c, size_t t, sf::Vector2i p)
	:code(new size_t(c))
	,type(new size_t(t))
	,ent_tag(nullptr)
	,ent_name(nullptr)
	,state(nullptr)
	,facing(nullptr)
	,pos(new sf::Vector2f(p))
	,dir(nullptr)
	{}

Action::Action(size_t c, size_t t, sf::Vector2f p, sf::Vector2f d)
	:code(new size_t(c))
	,type(new size_t(t))
	,ent_tag(nullptr)
	,ent_name(nullptr)
	,state(nullptr)
	,facing(nullptr)
	,pos(new sf::Vector2f(p))
	,dir(new sf::Vector2f(d))
	{}

Action::Action(size_t c, size_t t, sf::Vector2f p, size_t s, size_t f)
	:code(new size_t(c))
	,type(new size_t(t))
	,ent_tag(nullptr)
	,ent_name(nullptr)
	,state(new size_t(s))
	,facing(new size_t(f))
	,pos(new sf::Vector2f(p))
	,dir(nullptr)
	{}


std::ostream& operator << (std::ostream& os, const Action* a) {
    os << a->code << " " << a->type << " " << a->pos->x << " " << a->pos->y;
    return os;
}

Action::~Action() {
	delete code;
	delete type;
	delete ent_tag;
	delete ent_name;
	delete state;
	delete facing;
	delete pos;
	delete dir;
}
