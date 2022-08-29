#include "Game.h"
#include "SharedResources.h"

int main() {
	app_conf = new AppConfig();
	// load default settings
	app_conf->read("game.cfg");
	// load user settings
	app_conf->read("user.cfg");


	Game game;

	game.run();

	return 0;
}
