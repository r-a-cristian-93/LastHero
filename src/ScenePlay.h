#ifndef SCENE_PLAY
#define SCENE_PLAY

#include "Scene.h"
#include "Tilemap.h"
#include "Enums.h"
#include "MapCollision.h"
#include "Level.h"
#include "GameStats.h"
#include "ScenePlayData.h"
#include "SystemView.h"
#include "SystemStateFacing.h"
#include "SystemEntityPosition.h"
#include "SystemLifespan.h"
#include "SystemAnimation.h"


class ScenePlay: public Scene {
protected:
	ScenePlayData play_data;
	SystemView sView;
	SystemStateFacing sStateFacing;
	SystemEntityPosition sEntityPosition;
	SystemLifespan sLifespan;
	SystemAnimation sAnimation;

	void init();
	void load_level(std::string path);

	void spawnEnemy();
	void spawnEntity(size_t tag, size_t recipe_name, sf::Vector2f& pos, size_t state, size_t facing);
	void spawnEntity(size_t tag, size_t recipe_name, std::shared_ptr<Entity> owner, sf::Vector2f& pos, size_t state, size_t facing);
	void spawnMissle();
	void spawnExplosion(sf::Vector2f& pos);

	void sEnemySpawner();
	void sCollisionCheck();
	void sCollisionSolve();
	void sAI();
	void sFireWeapon();
	void sMissleGuidance();
	void sWidgetFx();
	void sInterface();
	void sGameState();
	void sPlayFx();
	void sPathFind();
	void sPowerup();

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
	void handleWidgetFx(Widget& w);

	void drawCollisionLayer();
	void drawDirectionVectors();
	void drawGrid();
	void drawEntityPosition();

public:
	void update() override;
	void doAction(const Action& a) override;

	ScenePlay(std::string lp);
	ScenePlay(size_t t, std::string lp);
	~ScenePlay();
};

#endif
