#ifndef SYSTEM_DRAW_ENTITIES
#define SYSTEM_DRAW_ENTITIES

#include <algorithm>
#include "System.h"
#include "ScenePlayData.h"
#include "SharedResources.h"
#include "Profiler.h"

class SystemDrawEntities: public System {
private:
	ScenePlayData& play_data;
	EntityVec& entities = play_data.ent_mgr.getEntities();

	// used only for drawing outline
	sf::RenderStates state_outline;
	sf::BlendMode blend_mode;

	sf::FloatRect ground_rect;
	sf::FloatRect view_rect;
	sf::View view;

	sf::RenderTexture clip_tex;
	sf::IntRect clip_rect;
	sf::Sprite clip_sprite;

	const sf::Sprite* anim_sprite;
	sf::Sprite overlay_sprite;

	sf::RenderTexture outline_tex;
	sf::Sprite outline_sprite;
	// end used only for drawing outline

public:
	SystemDrawEntities(ScenePlayData& _play_data)
		:play_data(_play_data)
	{
		state_outline = sf::RenderStates(&assets->getShader("outline"));
		blend_mode = sf::BlendMode(sf::BlendMode::Zero, sf::BlendMode::One, sf::BlendMode::Add, sf::BlendMode::DstAlpha, sf::BlendMode::OneMinusSrcAlpha, sf::BlendMode::Subtract);
		clip_tex.create(app_conf->game_w, app_conf->game_h);
	}

	void operator() () {
		PROFILE_SCOPE("DrawEntities");

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

		// update view_rect position
		ground_rect = play_data.level.map_ground.getBounds();
		view_rect = sf::FloatRect(
			play_data.cam.target.x-app_conf->game_w/2,
			play_data.cam.target.y-app_conf->game_h/2,
			app_conf->game_w,
			app_conf->game_h);

		//fix weird lines between map tiles when moving
		view_rect.left = round(view_rect.left);
		view_rect.top = round(view_rect.top);

		if (view_rect.left < 0) view_rect.left = 0;
		if (view_rect.top < 0) view_rect.top = 0;
		if (view_rect.left + view_rect.width > ground_rect.width) view_rect.left = ground_rect.width - app_conf->game_w;
		if (view_rect.top + view_rect.height > ground_rect.height) view_rect.top = ground_rect.height - app_conf->game_h;

		view.reset(view_rect);
		// end update view_rect position

		clip_tex.setView(view);
		clip_tex.clear(sf::Color::Transparent);
		clip_sprite = sf::Sprite(clip_tex.getTexture());

		std::reverse(entities.begin(), entities.end());

		for (const std::shared_ptr<Entity>& e:entities) {
			if (e->get<CAnimation>()) {
				anim_sprite = &e->get<CAnimation>()->active_anim->getSprite();

				if (e->tag != TAG::ENVIRONMENT && e->tag != TAG::FX && e->tag != TAG::PROJECTILE) {
					overlay_sprite = sf::Sprite(*anim_sprite);
					overlay_sprite.setPosition(0,0);
					overlay_sprite.setOrigin(0,0);

					clip_rect.width = anim_sprite->getLocalBounds().width;
					clip_rect.height = anim_sprite->getLocalBounds().height;
					clip_rect.left = anim_sprite->getGlobalBounds().left - view_rect.left;
					clip_rect.top = anim_sprite->getGlobalBounds().top - view_rect.top;

					clip_sprite.setTextureRect(clip_rect);
					clip_sprite.setPosition(0,0);
					clip_sprite.setOrigin(0,0);

					outline_tex.create(overlay_sprite.getLocalBounds().width, overlay_sprite.getLocalBounds().height);
					outline_tex.clear(sf::Color::Transparent);
					outline_tex.draw(overlay_sprite, state_outline);
					outline_tex.draw(clip_sprite, blend_mode);
					outline_tex.display();

					outline_sprite = sf::Sprite(outline_tex.getTexture());
					outline_sprite.setOrigin(anim_sprite->getOrigin());
					outline_sprite.setPosition(anim_sprite->getPosition());

					screen_tex->draw(outline_sprite);
				}

				if (e->tag != TAG::FX && e->tag != TAG::PROJECTILE && e->tag != TAG::PLAYER) {
					clip_tex.draw(*anim_sprite);
					clip_tex.display();
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