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
		FIRE_SECONDARY,
		GAME_PAUSE
	};

	enum {
		TYPE_END,
		TYPE_START
	};

	size_t code;
	size_t type;
	int frame;
	sf::Vector2f location;

	Action(size_t c, size_t t, int f);
	Action(size_t c, size_t t, sf::Vector2i l, int f);

	friend std::ostream& operator << (std::ostream& os, const Action* a);
};

#endif
