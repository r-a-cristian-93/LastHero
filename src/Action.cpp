#include "Action.h"

Action::Action()
	:code(nullptr)
	,type(nullptr)
	,frame(nullptr)
	,ent_tag(nullptr)
	,ent_id(nullptr)
	,pos(nullptr)
	,dir(nullptr)
	{}

Action::Action(size_t c, size_t t, int f)
	:code(new size_t(c))
	,type(new size_t(t))
	,frame(new int(f))
	{}

Action::Action(size_t c, size_t t, int f, sf::Vector2i p)
	:code(new size_t(c))
	,type(new size_t(t))
	,frame(new int(f))
	,pos(new sf::Vector2f(p))
	{}

Action::Action(size_t c, size_t t, int f, sf::Vector2i p, sf::Vector2f d)
	:code(new size_t(c))
	,type(new size_t(t))
	,frame(new int(f))
	,pos(new sf::Vector2f(p))
	,dir(new sf::Vector2f(d))
	{}


std::ostream& operator << (std::ostream& os, const Action* a) {
    os << a->code << " " << a->type << " " << a->frame << " " << a->pos->x << " " << a->pos->y;
    return os;
}
