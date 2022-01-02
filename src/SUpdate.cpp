#include "SUpdate.h"
#include <cmath>
#include <iostream>

void SUpdate::updatePosition(const EntityVec& entities, sf::FloatRect& limits) {
	for(const std::shared_ptr<Entity>& e:entities) {
		if (e->c_transform) {
			if (e->c_input) {
				if (e->c_input->up) e->c_transform->dir.y = -1;
				else if (e->c_input->down) e->c_transform->dir.y = 1;
				else e->c_transform->dir.y = 0;
				
				if (e->c_input->left) e->c_transform->dir.x = -1;
				else if (e->c_input->right) e->c_transform->dir.x = 1;
				else e->c_transform->dir.x = 0;				
			}
			
			if (e->c_transform->dir.x || e->c_transform->dir.y) {
				
				//set velocity according to direction and max_velocity;	
				float alpha_rad = atan2f(e->c_transform->dir.y, e->c_transform->dir.x);
				e->c_transform->vel.x = cos(alpha_rad) * e->c_transform->max_velocity;
				e->c_transform->vel.y = sin(alpha_rad) * e->c_transform->max_velocity;	

				sf::Vector2f pos_future = e->c_transform->pos + e->c_transform->vel;
				
				int r = e->c_shape->shape->getRadius();
				
				sf::FloatRect bounds;
				if (e->tag == Entity::TAG_MISSLE) {
					bounds = {pos_future.x+r, pos_future.y+r, pos_future.x-r, pos_future.y-r};				
				}
				else {
					bounds = {pos_future.x-r, pos_future.y-r, pos_future.x+r, pos_future.y+r};				
				}
				
				if (bounds.left <= limits.left || bounds.width >= limits.width) {
					if (e->c_input) {
						e->c_transform->dir.x = 0;
						e->c_transform->vel.x = 0;
					}
					else {
						e->c_transform->dir.x *= -1.0f;
						e->c_transform->vel.x *= -1.0f;
						
						if (e->tag == Entity::TAG_MISSLE) {
							e->alive = false;
						}
					}
				}

				if (bounds.top <= limits.top || bounds.height >= limits.height) {
					if (e->c_input) {
						e->c_transform->dir.y = 0;
						e->c_transform->vel.y = 0;
					}
					else {
						e->c_transform->dir.y *= -1.0f;
						e->c_transform->vel.y *= -1.0f;
						
						if (e->tag == Entity::TAG_MISSLE) {
							e->alive = false;
						}
					}
				}

				//update position
				e->c_transform->pos += e->c_transform->vel;
				
				if (e->c_shape) {
					sf::Vector2f velocity(e->c_transform->vel.x, e->c_transform->vel.y);
					e->c_shape->shape->move(velocity);
				}			
			}
		}
	}
}
