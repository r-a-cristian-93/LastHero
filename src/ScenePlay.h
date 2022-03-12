#ifndef SCENE_PLAY
#define SCENE_PLAY

#include "Scene.h"
#include "Interface.h"
#include "ParticlesEmitter.h"
#include "Tilemap.h"

struct Camera {
	sf::Vector2f pos, target;
};

class ScenePlay: public Scene {
	ParticlesEmitter glitter;

	std::string level_path;
	Tilemap map_ground;

	Camera cam;
	Interface interface;

	sf::View gui_view;

	std::shared_ptr<Entity> player;

	void init();
	void load_level(std::string path);

	void spawnPlayer();
	void spawnEnemy();
	void spawnEntity(size_t tag, std::string& recipe_name, sf::Vector2f& pos, size_t state, size_t facing);
	void spawnMissle();
	void spawnExplosion(sf::Vector2f& pos);
	void killEntity(std::shared_ptr<Entity>& entity);

	void sEnemySpawner();
	void sCollisionCheck();
	void sCollisionSolve();
	void sAI();
	void sFireWeapon();
	void sStateFacing();
	void sLifespan();
	void sMissleGuidance();
	void sPlayback();
	void sLevelUp();
	void sInterface();
	void sAnimation();
	void sView();

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
	~ScenePlay();
};

#endif
