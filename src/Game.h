#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <string>

#include "Entity.h"
#include "EntityManager.h"
#include "ActionManager.h"
#include "ActionStream.h"
#include "Common.h"

class Game {
	AppConfig app_conf;
	PlayerConfig p_conf;
	EnemyConfig e_conf;
	BulletConfig b_conf;
	MissleConfig m_conf;

	sf::RenderWindow window;
	sf::Font font;
	EntityManager ent_mgr;
	ActionManager act_mgr;
	ActionStream act_stream;

	int frame_current;
	int frame_last_spawn;
	int score;
	sf::Text score_text;

	bool paused, running;

	std::shared_ptr<Entity> player;

	void init(std::string file_name);
	void spawnPlayer();
	void spawnEnemy();
	void spawnBullet();
	void spawnChilds(const std::shared_ptr<Entity>& parent);
	void spawnMissle();

	void sEnemySpawner();
	void sUserInput();
	void sCollision();
	void sCombat();
	void sLifespan();
	void sSpin();
	void sMissleGuidance();

	void doAction(const Action& a);

	void checkLifespan(std::shared_ptr<Entity>& e);
	std::shared_ptr<Entity> findTarget(const std::shared_ptr<Entity>& missle);

	float angle(const sf::Vector2f a, const sf::Vector2f b);

public:
	Game(std::string file_name);

	void run();
};

#endif
