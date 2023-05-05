#include "Game.h"
#include "SharedResources.h"

int main() {
	app_conf = new AppConfig();
	cfg_mgr = new ConfigManager();
	act_mgr = new ActionManager();
	frame_mgr = new FrameManager();
	assets = new Assets();
	snd_mgr = new SoundManager();
	window = new sf::RenderWindow();
	screen_tex = new sf::RenderTexture();
	screen_sprite = new sf::Sprite();
	game_stats = new GameStats();

	app_conf->read("game.cfg"); 	// load default settings
	app_conf->read("user.cfg"); 	// load user settings
	screen_sprite->setTexture(screen_tex->getTexture());

	Game game;

	game.run();

	delete app_conf;
	delete cfg_mgr;
	delete act_mgr;
	delete snd_mgr;
	delete window;
	delete screen_tex;
	delete screen_sprite;
	delete game_stats;
	delete assets;

	return 0;
}
