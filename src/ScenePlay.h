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
#include "SystemSpawnFx.h"
#include "SystemWidgetFx.h"
#include "SystemFireWeapon.h"
#include "SystemAI.h"

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
	SystemSpawnFx sSpawnFx;
	SystemWidgetFx sWidgetFx;
	SystemFireWeapon sFireWeapon;
	SystemAI sAI;

	void init();
	void load_level(std::string path);

	void sGameState();

public:
	void update() override;
	void doAction(const Action& a) override;

	ScenePlay(std::string lp);
	ScenePlay(size_t t, std::string lp);
	~ScenePlay();
};

#endif
