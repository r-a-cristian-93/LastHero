#ifndef SCENE_PLAY
#define SCENE_PLAY

#include "Scene.h"

class ScenePlay: public Scene {
	std::string level_path;
	int score;
	sf::Text score_text;

	std::shared_ptr<Entity> player;

	void init();
	void load_level();

	void spawnPlayer();
	void spawnEnemy();
	void spawnBullet();
	void spawnChilds(const std::shared_ptr<Entity>& parent);
	void spawnMissle();

	void sEnemySpawner();
	void sCollision();
	void sCombat();
	void sLifespan();
	void sSpin();
	void sMissleGuidance();
	void sPlayback();

	void checkLifespan(std::shared_ptr<Entity>& e);
	std::shared_ptr<Entity> findTarget(const std::shared_ptr<Entity>& missle);

	float angle(const sf::Vector2f a, const sf::Vector2f b);
	float squareDistance(const sf::Vector2f& a, const sf::Vector2f& b);
	bool checkCollision(std::shared_ptr<Entity>& a, std::shared_ptr<Entity>& b);

public:
	void update() override;
	void doAction(const Action* a) override;

	ScenePlay();
	ScenePlay(Game* g, std::string lp);
};

#endif
