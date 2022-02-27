#include "SUpdate.h"
#include <cmath>
#include <iostream>

void SUpdate::updatePosition(const EntityVec& entities, const sf::FloatRect& limits) {
	for(const std::shared_ptr<Entity>& e:entities) {
		if (e->get<CTransform>()) {
			sf::Vector2f& prev_dir = e->get<CTransform>()->prev_dir;
			sf::Vector2f& prev_pos = e->get<CTransform>()->prev_pos;
			sf::Vector2f& dir = e->get<CTransform>()->dir;
			sf::Vector2f& vel = e->get<CTransform>()->vel;
			sf::Vector2f& pos = e->get<CTransform>()->pos;

			prev_pos = pos;

			if (e->get<CInput>()) {
				if (e->get<CInput>()->up) dir.y = -1;
				else if (e->get<CInput>()->down) dir.y = 1;
				else dir.y = 0;

				if (e->get<CInput>()->left) dir.x = -1;
				else if (e->get<CInput>()->right) dir.x = 1;
				else dir.x = 0;
			}

			if ((dir.x || dir.y) && e->get<CTransform>()->max_velocity) {

				//set velocity according to direction and max_velocity;
				float alpha_rad = atan2f(dir.y, dir.x);
				vel.x = cos(alpha_rad) * e->get<CTransform>()->max_velocity;
				vel.y = sin(alpha_rad) * e->get<CTransform>()->max_velocity;

				//chech if it collides with boundaries
				if (e->get<CCollision>()) {
					const int r = e->get<CCollision>()->radius;
					sf::Vector2f pos_future = pos + vel;

					sf::FloatRect bounds;
					if (e->tag == Entity::TAG_MISSLE) {
						bounds = {pos_future.x+r, pos_future.y+r, pos_future.x-r, pos_future.y-r};
					}
					else {
						bounds = {pos_future.x-r, pos_future.y-r, pos_future.x+r, pos_future.y+r};
					}

					if (bounds.left <= limits.left || bounds.width >= limits.width) {
						if (e->get<CInput>()) {
							dir.x = 0;
							vel.x = 0;
						}
						else {
							dir.x *= -1.0f;
							vel.x *= -1.0f;

							if (e->tag == Entity::TAG_MISSLE) {
								e->alive = false;
							}
						}
					}

					if (bounds.top <= limits.top || bounds.height >= limits.height) {
						if (e->get<CInput>()) {
							dir.y = 0;
							vel.y = 0;
						}
						else {
							dir.y *= -1.0f;
							vel.y *= -1.0f;

							if (e->tag == Entity::TAG_MISSLE) {
								e->alive = false;
							}
						}
					}
				}

				//update position
				pos += vel;

				if (e->get<CShape>()) {
					e->get<CShape>()->shape.setPosition(pos + e->get<CCollision>()->offset[e->facing]);
				}

				//updates only if there is a change in direction
				prev_dir = dir;
			}
			else {
				vel.x = 0;
				vel.y = 0;
			}
		}
	}
}
