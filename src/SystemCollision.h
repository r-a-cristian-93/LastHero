#ifndef SYSTEM_COLLISION_H
#define SYSTEM_COLLISION_H

#include "System.h"
#include "ScenePlayData.h"
#include "CheckCollision.h"

class SystemCollision: public System {
private:
	ScenePlayData& play_data;

public:
	SystemCollision(ScenePlayData& _play_data): play_data(_play_data) {}

	void operator() () {
		sCollisionCheck();
		sCollisionSolve();
	}

	void sCollisionCheck() {
		EntityVec& entities = play_data.ent_mgr.getEntities();

		// clear colliders
		for (size_t i=0; i<entities.size(); i++) {
			if (entities[i]->get<CCollision>()) {
				entities[i]->get<CCollision>()->colliders.clear();
				entities[i]->get<CCollision>()->hitboxes_this.clear();
				entities[i]->get<CCollision>()->hitboxes_collider.clear();
			}
		}

		for (size_t i=0; i<entities.size(); i++) {
			for (size_t j=i+1; j<entities.size(); j++) {
				CheckCollision::of(entities[i], entities[j]);
			}
		}
	}

	void sCollisionSolve() {
		for (std::shared_ptr<Entity>& entity : play_data.ent_mgr.getEntities()) {

			// if it's not a projectile and has CCollision
			if (entity->tag != TAG::PROJECTILE &&
				entity->tag != TAG::POWERUP &&
				entity->tag != TAG::FX &&
				entity->get<CCollision>() && entity->alive)
			{

				EntityVec& colliders = entity->get<CCollision>()->colliders;
				std::vector<HitBox*>& hitboxes_this = entity->get<CCollision>()->hitboxes_this;
				std::vector<HitBox*>& hitboxes_collider = entity->get<CCollision>()->hitboxes_collider;

				// if collided with something
				if (colliders.size() > 0) {
					for (size_t i = 0; i < colliders.size(); i++) {

						// skip if owner spawned the projectile
						if (colliders[i]->owner) {
							if (colliders[i]->owner == entity) {
								continue;
							}
						}

						// skip if collider is POWERUP
						if (colliders[i]->tag == TAG::POWERUP) {
							continue;
						}

						// if the collider[i] is a projectile apply damage and kill colliders[i]
						if (colliders[i]->tag == TAG::PROJECTILE && colliders[i]->alive) {
							colliders[i]->alive = false;

							// hit flag is set only for one frame
							// it will be unset by sStateFacing
							entity->hit = true;

							if (entity->get<CStats>() && colliders[i]->get<CStats>()) {
								const int& collider_atk = colliders[i]->get<CStats>()->effective[CStats::ATTACK];
								int& entity_hp = entity->get<CStats>()->effective[CStats::HEALTH];
								int& entity_def = entity->get<CStats>()->effective[CStats::DEFENCE];

								if (entity_def > 0) {
									entity_def -= collider_atk;

									if (entity_def < 0) {
										entity_hp -= -entity_def;
										entity_def = 0;
									}
								}
								else {
									entity_hp -= collider_atk;
								}

								if (entity_hp <= 0) {
									entity_hp = 0;
									entity->alive = false;

									if (colliders[i]->owner) {
										if (colliders[i]->owner == play_data.player && entity->tag == TAG::ENEMY) {
											play_data.kills_per_enemy[entity->name]++;
											play_data.total_kills++;
										}
									}
								}

								if (entity->get<CSfx>()) {
									if (entity_hp <= 0) {
										if (entity->get<CSfx>()->die) {
											snd_mgr->playSoundUnique(entity->get<CSfx>()->die);
										}
									}
									else {
										if (entity->get<CSfx>()->hurt) {
											snd_mgr->playSoundUnique(entity->get<CSfx>()->hurt);
										}
									}
								}
							}
						}

						// else if it's not a collider(projectile) move to previous position
						else {
							// if the entity is able to move and the collider it's not a projectile;
							entity->get<CTransform>()->pos = entity->get<CTransform>()->prev_pos;

							if (entity->get<CTransform>()->max_velocity && !colliders[i]->get<CLifespan>()) {

								const sf::Vector2f& e_pos = entity->get<CTransform>()->pos + hitboxes_this[i]->offset[entity->facing];
								const sf::Vector2f& c_pos = colliders[i]->get<CTransform>()->pos + hitboxes_collider[i]->offset[colliders[i]->facing];
								const sf::Vector2f delta = e_pos - c_pos;

								float dx = 0;
								float dy = 0;

								float velocity = entity->get<CTransform>()->max_velocity;

								if (delta.y > 0) dy += velocity;
								else if (delta.y < 0) dy -= velocity;

								if (delta.x > 0) dx += velocity;
								else if (delta.x < 0) dx -= velocity;

								switch (entity->facing) {
									case Entity::FACING_E:
									case Entity::FACING_W:
										dx = 0;
									break;
									case Entity::FACING_N:
									case Entity::FACING_S:
										dy = 0;
									break;
									case Entity::FACING_NE:
									case Entity::FACING_NW:
									case Entity::FACING_SE:
									case Entity::FACING_SW:
										if (abs(delta.x) > abs(delta.y))
											dy = 0;
										else
											dx = 0;
									break;
								}

								entity->get<CTransform>()->pos.x += dx;
								entity->get<CTransform>()->pos.y += dy;

							}
						}
					}
				}
			}
		}
	}
};

#endif