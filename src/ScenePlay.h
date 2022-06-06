#ifndef SCENE_PLAY
#define SCENE_PLAY

#include "Scene.h"
#include "Interface.h"
#include "ParticlesEmitter.h"
#include "Tilemap.h"
#include "Enums.h"
#include "MapCollision.h"

struct Camera {
	sf::Vector2f pos, target;
};

typedef size_t GameState;

class ScenePlay: public Scene {
	enum {
		GAME_INTRO,
		GAME_PLAY,
		GAME_OVER
	};

	ParticlesEmitter glitter;

	std::string level_path;
	sf::Vector2u tile_size;
	sf::Vector2u map_size;

	Tilemap map_ground;
	MapCollision collision_map;

	Camera cam;
	Interface interface;
	int total_kills;
	KillsMap kills_per_enemy;
	GameState game_state;

	std::shared_ptr<Entity> player;
	std::shared_ptr<Entity> base;

	void init();
	void load_level(std::string path);

	void spawnPlayer();
	void spawnBase();
	void spawnEnemy();
	void spawnEntity(size_t tag, size_t recipe_name, sf::Vector2f& pos, size_t state, size_t facing);
	void spawnEntity(size_t tag, size_t recipe_name, std::shared_ptr<Entity> owner, sf::Vector2f& pos, size_t state, size_t facing);
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
	void sGameState();
	void sPlayFx();
	void sPathFind();

	void checkLifespan(std::shared_ptr<Entity>& e);
	std::shared_ptr<Entity> findTarget(const std::shared_ptr<Entity>& missle);

	float angle(const sf::Vector2f a, const sf::Vector2f b);
	float angle(sf::Vector2f v);
	size_t facingOf(sf::Vector2f v);
	sf::Vector2f dirOf(size_t facing);
	void lookAt(CInput& c_input, const sf::Vector2f& a, const sf::Vector2f& b);
	float squareDistance(const sf::Vector2f& a, const sf::Vector2f& b);
	bool checkCollision(std::shared_ptr<Entity>& a, std::shared_ptr<Entity>& b);
	bool checkCollision(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b, const size_t threshold);

	void handleFire(std::shared_ptr<Entity>& e, const BCondition& bc, bool& fire_weapon);
	void handleChase(std::shared_ptr<Entity>& e, const BCondition& bc);

	void drawCollisionLayer();

public:
	void update() override;
	void doAction(const Action* a) override;

	ScenePlay();
	ScenePlay(Game* g, std::string lp);
	~ScenePlay();
};

#endif
