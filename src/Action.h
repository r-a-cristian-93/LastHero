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
		SPAWN_BASE,
		SPAWN_ENTITY,

		GAME_PAUSE,
		GAME_EXIT,
		MENU_SELECT,
		MENU_SKIP,

		CHANGE_SCENE_MENU,
		CHANGE_SCENE_PLAY,
		CHANGE_SCENE_EDITOR,
	};

	enum {
		TYPE_END,
		TYPE_START
	};

	size_t* code;
	size_t* type;
	size_t* ent_tag;
	size_t* ent_name;
	size_t* state;
	size_t* facing;
	sf::Vector2f* pos;

	Action();
	Action(const Action& action);
	Action(size_t c, size_t t);
	Action(size_t c, size_t t, sf::Vector2i p);
	Action(size_t c, size_t t, sf::Vector2f p, size_t s, size_t f);
	~Action();

	friend std::ostream& operator << (std::ostream& os, const Action& a);
};

#endif
