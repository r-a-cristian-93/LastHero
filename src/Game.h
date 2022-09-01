#ifndef GAME
#define GAME

#include "App.h"

class Game: public App {
private:

	void init() override;
	void setScene(size_t id) override;

public:

	Game();
	~Game();
};

#endif
