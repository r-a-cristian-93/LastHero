#ifndef ACTION
#define ACTION

#include <SFML/Graphics.hpp>
#include <iostream>

class Action {
public:
	//action codes
	enum {
		NONE,
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN,
		FIRE_PRIMARY,
		FIRE_SECONDARY
	};

	enum {
		TYPE_END,
		TYPE_START
	};

	int code;
	int type;
	int frame;
	sf::Vector2f location;

	Action(int c, int t, int f);
	Action(int c, int t, sf::Vector2i l, int f);

	friend std::ostream& operator<<(std::ostream& os, const Action& a);
};

#endif
