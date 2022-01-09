#include "Action.h"

Action::Action()
	:code(nullptr)
	,type(nullptr)
	,frame(nullptr)
	,ent_tag(nullptr)
	,ent_id(nullptr)
	,pos_x(nullptr)
	,pos_y(nullptr)
	,dir_x(nullptr)
	,dir_y(nullptr)
	{}

Action::Action(size_t c, size_t t, int f)
	:code(new size_t(c))
	,type(new size_t(t))
	,location(0, 0)
	,frame(new int(f))
	{}

Action::Action(size_t c, size_t t, sf::Vector2i l, int f)
	:code(new size_t(c))
	,type(new size_t(t))
	,location(l)
	,frame(new int(f))
	{}


std::ostream& operator << (std::ostream& os, const Action* a) {
    os << a->code << " " << a->type << " " << a->frame << " " << a->location.x << " " << a->location.y;
    return os;
}
