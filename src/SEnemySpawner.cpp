#include "SEnemySpawner.h"

void SEnemySpawner::spawnEnemy(EntityManager& em) {
	std::shared_ptr<Entity> e = em.add(TAG::ENEMY);

}
