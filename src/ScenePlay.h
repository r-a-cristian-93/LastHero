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
	Widget* status_widget;
	sf::View gui_view;

	WidgetText* w_health_text;
	WidgetText* w_defence_text;

	Widget* score_widget;
	std::string score_text;
	int score;

	Widget* wave_widget;
	std::string wave_text;
	int wave_current, wave_total;

	std::shared_ptr<Entity> player;

	void init();
	void load_level(std::string path);

	void spawnPlayer();
	void spawnEnemy();
	void spawnEnemy(size_t tag, std::string& recipe_name, sf::Vector2f& pos, sf::Vector2f& dir);
	void spawnBullet(std::string& recipe_name);
	void spawnChilds(const std::shared_ptr<Entity>& parent);
	void spawnMissle();
	void spawnExplosion(sf::Vector2f& pos);

	void sEnemySpawner();
	void sCollisionCheck();
	void sCollisionSolve();
	void sCombat();
	void sLifespan();
	void sSpin();
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
