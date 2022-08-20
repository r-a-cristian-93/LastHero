#include "Action.h"

Action::Action()
	:code(nullptr)
	,type(nullptr)
	,ent_tag(nullptr)
	,ent_name(nullptr)
	,state(nullptr)
	,facing(nullptr)
	,pos(nullptr)
	{}
Action::Action(const Action& a)
	:code(new size_t(*a.code))
	,type(new size_t(*a.type))
	,ent_tag(new size_t(*a.ent_tag))
	,ent_name(new size_t(*a.ent_name))
	,state(new size_t(*a.state))
	,facing(new size_t(*a.facing))
	,pos(new sf::Vector2f(*a.pos))
	{}

Action::Action(size_t c, size_t t)
	:code(new size_t(c))
	,type(new size_t(t))
	,ent_tag(nullptr)
	,ent_name(nullptr)
	,state(nullptr)
	,facing(nullptr)
	,pos(nullptr)
	{}

Action::Action(size_t c, size_t t, sf::Vector2i p)
	:code(new size_t(c))
	,type(new size_t(t))
	,ent_tag(nullptr)
	,ent_name(nullptr)
	,state(nullptr)
	,facing(nullptr)
	,pos(new sf::Vector2f(p))
	{}

Action::Action(size_t c, size_t t, sf::Vector2f p, size_t s, size_t f)
	:code(new size_t(c))
	,type(new size_t(t))
	,ent_tag(nullptr)
	,ent_name(nullptr)
	,state(new size_t(s))
	,facing(new size_t(f))
	,pos(new sf::Vector2f(p))
	{}


std::ostream& operator << (std::ostream& os, const Action& a) {
	if (a.code) os << "Code " << *a.code << " ";
	if (a.type) os << "Type " << *a.type << " ";
	if (a.ent_tag) os << "Ent tag " << *a.ent_tag << " ";
	if (a.ent_name) os << "Ent name " << *a.ent_name << " ";
	if (a.state) os << "Ent state " << *a.state << " ";
	if (a.facing) os << "Ent facing " << *a.facing << " ";
	if (a.pos) os << "Pos " << a.pos->x << "x" << a.pos->y << " ";

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
}
