#ifndef SCENE_PLAY
#define SCENE_PLAY

#include "Scene.h"
#include "Interface.h"

class ScenePlay: public Scene {
	std::string level_path;

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
	void spawnEnemy(size_t tag, size_t recipe_id, sf::Vector2f pos, sf::Vector2f dir);
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
	void sLevelUp();
	void sInterface();
	void sAnimation();
	void sView();

	void checkLifespan(std::shared_ptr<Entity>& e);
	void setStatsInitial(Entity& entity);
	void setStatsEffective(Entity& entity);
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
