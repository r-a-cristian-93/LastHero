#ifndef CHECK_COLLISION_H
#define CHECK_COLLISION_H

#include "Entity.h"

namespace CheckCollision {
	bool of(std::shared_ptr<Entity>& a, std::shared_ptr<Entity>& b) {
		bool collision = false;

		if (a->get<CCollision>() && b->get<CCollision>()) {
			if (!a->get<CCollision>()->hitbox.empty() && !b->get<CCollision>()->hitbox.empty()) {
				for (HitBox& hb_a : a->get<CCollision>()->hitbox) {
					for (HitBox& hb_b : b->get<CCollision>()->hitbox) {
						sf::Vector2f pos_a = a->get<CTransform>()->pos + hb_a.offset[a->facing];
						sf::Vector2f pos_b = b->get<CTransform>()->pos + hb_b.offset[b->facing];

						float square_distance = squareDistance(pos_a, pos_b);
						int square_radius = (hb_a.radius + hb_b.radius) * (hb_a.radius + hb_b.radius);

						if (square_distance < square_radius) {
							collision = true;

							a->get<CCollision>()->colliders.push_back(b);
							a->get<CCollision>()->hitboxes_this.push_back(&hb_a);
							a->get<CCollision>()->hitboxes_collider.push_back(&hb_b);

							b->get<CCollision>()->colliders.push_back(a);
							b->get<CCollision>()->hitboxes_this.push_back(&hb_b);
							b->get<CCollision>()->hitboxes_collider.push_back(&hb_a);
						}
					}
				}
			}
		}

		return collision;
	}

	bool of(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b, const size_t threshold) {
		bool collision = false;

		if (a->get<CCollision>() && b->get<CCollision>()) {
			if (!a->get<CCollision>()->hitbox.empty() && !b->get<CCollision>()->hitbox.empty()) {
				for (HitBox& hb_a : a->get<CCollision>()->hitbox) {
					for (HitBox& hb_b : b->get<CCollision>()->hitbox) {
						sf::Vector2f pos_a = a->get<CTransform>()->pos + hb_a.offset[a->facing];
						sf::Vector2f pos_b = b->get<CTransform>()->pos + hb_b.offset[b->facing];

						float square_distance = squareDistance(pos_a, pos_b);
						int square_radius = (hb_a.radius + hb_b.radius + threshold) * (hb_a.radius + hb_b.radius + threshold);

						if (square_distance < square_radius) {
							collision = true;
						}
					}
				}
			}
		}

		return collision;
	}

	float squareDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
		return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
	}
};

#endif