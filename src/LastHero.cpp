#include "Game.h"
#include "SharedResources.h"

int main() {
	app_conf = new AppConfig();
	// load default settings
	app_conf->read("game.cfg");
	// load user settings
	app_conf->read("user.cfg");

	act_mgr = new ActionManager();


	Game game;

	game.run();

	delete app_conf;
	delete act_mgr;

	return 0;
}
