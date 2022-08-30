#include "Game.h"
#include "SharedResources.h"

int main() {
	app_conf = new AppConfig();
	// load default settings
	app_conf->read("game.cfg");
	// load user settings
	app_conf->read("user.cfg");

	act_mgr = new ActionManager();

	assets = new Assets();

	snd_mgr = new SoundManager();

	window = new sf::RenderWindow();

	screen_tex = new sf::RenderTexture();

	screen_sprite = new sf::Sprite();
	screen_sprite->setTexture(screen_tex->getTexture());

	game_view = new sf::View();

	Game game;

	game.run();

	delete app_conf;
	delete act_mgr;
	delete assets;

	return 0;
}
