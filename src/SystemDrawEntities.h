#ifndef SYSTEM_DRAW_ENTITIES
#define SYSTEM_DRAW_ENTITIES

#include <algorithm>
#include "System.h"
#include "ScenePlayData.h"
#include "SharedResources.h"

class SystemDrawEntities: public System {
private:
	ScenePlayData& play_data;
	EntityVec& entities = play_data.ent_mgr.getEntities();

public:
	SystemDrawEntities(ScenePlayData& _play_data)
		:play_data(_play_data)
	{}

	void operator() () {
		std::sort(entities.begin(), entities.end(), comparePosition);

		for (const std::shared_ptr<Entity>& e:entities) {
			if (e->get<CAnimation>()) {
				screen_tex->draw(e->get<CAnimation>()->active_anim->getSprite());
			}

			#ifdef DRAW_COLLISION_BOX
			if (e->get<CShape>() && e->get<CCollision>()) {
				sf::Vector2f& pos = e->get<CTransform>()->pos;

				for (HitBox& hb : e->get<CCollision>()->hitbox) {
					e->get<CShape>()->shape.setRadius(hb.radius);
					e->get<CShape>()->shape.setOrigin(hb.radius, hb.radius);
					e->get<CShape>()->shape.setPosition(pos + hb.offset[e->facing]);
					screen_tex->draw(e->get<CShape>()->shape);
				}
			}
			#endif
		}

		// render and draw outlines
		for (const std::shared_ptr<Entity>& e:entities) {
			if (e->tag != TAG::ENVIRONMENT && e->tag != TAG::FX && e->tag != TAG::PROJECTILE) {
				if (e->get<CAnimation>()) {
					sf::RenderStates render_states = e->get<CAnimation>()->outline_render_states;
					const sf::Sprite& outline_sprite = e->get<CAnimation>()->active_anim->getSprite();

					screen_tex->draw(outline_sprite, render_states);
				}
			}
		}

		#ifdef DEBUG_GRID
			drawGrid();
		#endif

		#ifdef DEBUG_COLLISION_LAYER
			drawCollisionLayer();
		#endif

		#ifdef DEBUG_DIRECTION
			drawDirectionVectors();
		#endif

		#ifdef DEBUG_ENTITY_POS
			drawEntityPosition();
		#endif
	}

private:
	static bool comparePosition(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
		if (a->get<CAnimation>() && b->get<CAnimation>()) {
			if (a->get<CAnimation>()->prio != b->get<CAnimation>()->prio) {
				return (a->get<CAnimation>()->prio < b->get<CAnimation>()->prio);
			}
		}

		if (a->get<CTransform>() && b->get<CTransform>()) {
			return (a->get<CTransform>()->pos.y < b->get<CTransform>()->pos.y);
		}

		return false;
	}

	void drawCollisionLayer() {
		sf::CircleShape circle(play_data.level.tile_size.x/(app_conf->colmap_res*2));

		//DRAW BLOCKING
		if (!play_data.collision_map.colmap.empty()) {
			circle.setFillColor(sf::Color(255, 0, 0, 50));

			for (int y = 0; y < play_data.collision_map.map_size.x; y++) {
				for (int x = 0; x < play_data.collision_map.map_size.x; x++) {
					circle.setPosition(sf::Vector2f(x*play_data.level.tile_size.x/app_conf->colmap_res,y*play_data.level.tile_size.y/app_conf->colmap_res));

					if (play_data.collision_map.colmap[x][y]) {
						screen_tex->draw(circle);
					}
				}
			}
		}

		//DRAW PATHS
		circle.setFillColor(sf::Color(0, 255, 255, 80));

		for (std::shared_ptr<Entity>& e: play_data.ent_mgr.getEntities()) {
			if (e->get<CBChase>()) {
				if (!e->get<CBChase>()->path.empty()) {
					for (sf::Vector2f p: e->get<CBChase>()->path) {
						circle.setPosition(sf::Vector2f(p.x, p.y));
						screen_tex->draw(circle);
					}
				}
			}
		}
	}

	void drawDirectionVectors() {
		for (std::shared_ptr<Entity>& e: play_data.ent_mgr.getEntities()) {
			if (e->get<CTransform>()) {
				if (e->get<CTransform>()->max_velocity) {
					sf::Vector2f dir = dirOf(e->facing);
					dir.x *= play_data.level.tile_size.x/2;
					dir.y *= play_data.level.tile_size.x/2;

					sf::Vertex line[] = {
						sf::Vertex(e->get<CTransform>()->pos),
						sf::Vertex(e->get<CTransform>()->pos + dir)
					};
					screen_tex->draw(line, 2, sf::Lines);
				}
			}
		}
	}

	void drawGrid() {
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(play_data.level.tile_size));
		rect.setFillColor(sf::Color(0,0,0,0));
		rect.setOutlineColor(sf::Color(0,0,0,50));
		rect.setOutlineThickness(-1);

		for (size_t x = 0; x < play_data.level.map_size.x; x++) {
			for (size_t y = 0; y < play_data.level.map_size.y; y++) {
				rect.setPosition(sf::Vector2f(x*play_data.level.tile_size.x, y*play_data.level.tile_size.y));
				screen_tex->draw(rect);
			}
		}
	}

	void drawEntityPosition() {
		sf::Color col(255,0,0);
		for (std::shared_ptr<Entity>& e : play_data.ent_mgr.getEntities()) {
			if (e->get<CTransform>()) {
				sf::Vector2f pos = e->get<CTransform>()->pos;
				float d = 10;

				sf::Vertex crosshair[] {
					sf::Vertex(pos - sf::Vector2f(d,0), col),
					sf::Vertex(pos + sf::Vector2f(d,0), col),
					sf::Vertex(pos - sf::Vector2f(0,d), col),
					sf::Vertex(pos + sf::Vector2f(0,d), col)
				};

				screen_tex->draw(crosshair, 4, sf::Lines);
			}
		}
	}

	sf::Vector2f dirOf(size_t facing) {
		switch (facing) {
			case Entity::FACING_E: return {1, 0}; break;
			case Entity::FACING_NE: return {1, -1}; break;
			case Entity::FACING_N: return {0, -1}; break;
			case Entity::FACING_NW: return {-1, -1}; break;
			case Entity::FACING_W: return {-1, 0}; break;
			case Entity::FACING_SW: return {-1, 1}; break;
			case Entity::FACING_S: return {0, 1}; break;
			case Entity::FACING_SE: return {1, 1}; break;
			default: return {0,0};
		}
	}
};

#endif