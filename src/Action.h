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

		SPAWN_PLAYER,
		SPAWN_ENEMY,

		GAME_PAUSE,
		GAME_EXIT,
		MENU_SELECT,

		CHANGE_SCENE_MENU,
		CHANGE_SCENE_PLAY,
	};

	enum {
		TYPE_END,
		TYPE_START
	};

	size_t* code;
	size_t* type;
	size_t* ent_tag;
	size_t* state;
	size_t* facing;
	std::string* ent_name;
	sf::Vector2f* pos;
	sf::Vector2f* dir;


	Action();
	Action(size_t c, size_t t);
	Action(size_t c, size_t t, sf::Vector2i p);
	Action(size_t c, size_t t, sf::Vector2f p, sf::Vector2f d);
	Action(size_t c, size_t t, sf::Vector2f p, size_t s, size_t f);

	friend std::ostream& operator << (std::ostream& os, const Action* a);
};

#endif
