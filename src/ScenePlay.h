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
#include "SystemDrawEntities.h"
#include "SystemPowerup.h"
#include "SystemCollision.h"

class ScenePlay: public Scene {
protected:
	ScenePlayData play_data;
	SystemView sView;
	SystemStateFacing sStateFacing;
	SystemEntityPosition sEntityPosition;
	SystemLifespan sLifespan;
	SystemAnimation sAnimation;
	SystemDrawEntities sDrawEntities;
	SystemPowerup sPowerup;
	SystemCollision sCollision;

	void init();
	void load_level(std::string path);

	void sAI();
	void sFireWeapon();
	void sMissleGuidance();
	void sWidgetFx();
	void sGameState();
	void sPlayFx();

	std::shared_ptr<Entity> findTarget(const std::shared_ptr<Entity>& missle);

	float angle(const sf::Vector2f a, const sf::Vector2f b);
	float angle(sf::Vector2f v);
	size_t facingOf(sf::Vector2f v);
	sf::Vector2f dirOf(size_t facing);
	void lookAt(CInput& c_input, const sf::Vector2f& a, const sf::Vector2f& b);
	float squareDistance(const sf::Vector2f& a, const sf::Vector2f& b);

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
