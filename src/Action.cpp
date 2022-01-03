#include "Action.h"

Action::Action(int c, int t, int f)
	:code(c)
	,type(t)
	,location(0,0)
	,frame(f)
	{}

Action::Action(int c, int t, sf::Vector2i l, int f)
	:code(c)
	,type(t)
	,location(l)
	,frame(f)
	{}

std::ostream& operator<<(std::ostream& os, const Action& a)
{
    os << "Code: " << a.code << " Type: " << a.type << " Frame: " << a.frame << " Location: " << a.location.x << "x" << a.location.y;
    return os;
}
