#include "Action.h"

Action::Action(size_t c, size_t t, int f)
	:code(c)
	,type(t)
	,location(0, 0)
	,frame(f)
	{}

Action::Action(size_t c, size_t t, sf::Vector2i l, int f)
	:code(c)
	,type(t)
	,location(l)
	,frame(f)
	{}


std::ostream& operator << (std::ostream& os, const Action* a) {
    os << a->code << " " << a->type << " " << a->frame << " " << a->location.x << " " << a->location.y;
    return os;
}
