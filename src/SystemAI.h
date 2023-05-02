#ifndef SYSTEM_AI_H
#define SYSTEM_AI_H

#include <math.h>
#include "System.h"
#include "ScenePlayData.h"
#include "CheckCollision.h"

class SystemAI: public System {
private:
	ScenePlayData& play_data;

public:
	SystemAI(ScenePlayData& _play_data): play_data(_play_data)
	{}


	void operator() () {
		for (std::shared_ptr<Entity>& e : play_data.ent_mgr.getEntities()) {
			bool has_target = false;

			if (e->get<CBChase>()) {
				for (BCondition& bc:e->get<CBChase>()->cond) {
					handleChase(e, bc);
				}

				if (e->get<CBChase>()->target && e->tag != TAG::ENVIRONMENT) {
					std::shared_ptr<Entity>& t = e->get<CBChase>()->target;
					has_target = true;

					sf::Vector2f start = e->get<CTransform>()->pos;
					if (e->get<CCollision>()) {
						if (!e->get<CCollision>()->hitbox.empty()) {
							start += e->get<CCollision>()->hitbox[0].offset[e->facing];
						}
					}

					sf::Vector2f end = t->get<CTransform>()->pos;

					std::vector<sf::Vector2f>& path = e->get<CBChase>()->path;
					bool has_path = play_data.collision_map.computePath(start, end, path, MapCollision::MOVE_NORMAL, 0);

					// move twards target if it's not too close;
					size_t range = 0;
					if (e->get<CWeapon>()) {
						size_t p_r = e->get<CWeapon>()->p_range;
						size_t s_r = e->get<CWeapon>()->s_range;

						if (p_r) range = p_r;
						if (s_r && s_r < p_r) range = s_r;
					}

					if (has_path) {
						lookAt(*e->get<CInput>(), path.back(), start);
					}
					else {
						lookAt(*e->get<CInput>(), e->get<CBChase>()->target->get<CTransform>()->pos, e->get<CTransform>()->pos);
					}

					if (CheckCollision::of(e, e->get<CBChase>()->target, range*0.2)) {
						e->get<CInput>()->right = false;
						e->get<CInput>()->left = false;
						e->get<CInput>()->up = false;
						e->get<CInput>()->down = false;
					}
				}
			}

			if (e->get<CBFire>() && e->get<CWeapon>() && e->get<CInput>()) {
				if (e->get<CWeapon>()->primary) {
					for (BCondition& bc: e->get<CBFire>()->pri) {
						handleFire(e, bc, e->get<CInput>()->fire_primary);
					}
				}
				if (e->get<CWeapon>()->secondary) {
					for (BCondition& bc: e->get<CBFire>()->sec) {
						handleFire(e, bc, e->get<CInput>()->fire_secondary);
					}
				}
			}



			if (e->get<CBPatrol>() && e->get<CInput>() && !has_target) {
				switch (e->get<CBPatrol>()->patrol) {
					case CBPatrol::PATROL_HORIZONTAL:
						if (e->get<CTransform>()->pos.x - e->get<CBPatrol>()->base_pos.x <= 0) {
							e->get<CInput>()->right = true;
							e->get<CInput>()->left = false;
						}
						else if (e->get<CTransform>()->pos.x - e->get<CBPatrol>()->base_pos.x >= e->get<CBPatrol>()->dist) {
							e->get<CInput>()->right = false;
							e->get<CInput>()->left = true;
						}

						// if it has lost y position
						if (e->get<CTransform>()->pos.y - e->get<CBPatrol>()->base_pos.y >= 30) {
							e->get<CInput>()->up = true;
							e->get<CInput>()->down = false;
						}
						else if (e->get<CTransform>()->pos.y - e->get<CBPatrol>()->base_pos.y <= -30) {
							e->get<CInput>()->up = false;
							e->get<CInput>()->down = true;
						}
						else {
							e->get<CInput>()->up = false;
							e->get<CInput>()->down = false;
						}
					break;

					case CBPatrol::PATROL_VERTICAL:
						if (e->get<CTransform>()->pos.y - e->get<CBPatrol>()->base_pos.y <= 0) {
							e->get<CInput>()->up = false;
							e->get<CInput>()->down = true;
						}
						else if (e->get<CTransform>()->pos.y - e->get<CBPatrol>()->base_pos.y >= e->get<CBPatrol>()->dist) {
							e->get<CInput>()->up = true;
							e->get<CInput>()->down = false;
						}

						// if it has lost x position
						if (e->get<CTransform>()->pos.x - e->get<CBPatrol>()->base_pos.x >= 30) {
							e->get<CInput>()->left = true;
							e->get<CInput>()->right = false;
						}
						else if (e->get<CTransform>()->pos.x - e->get<CBPatrol>()->base_pos.x <= -30) {
							e->get<CInput>()->left = false;
							e->get<CInput>()->right = true;
						}
						else {
							e->get<CInput>()->left = false;
							e->get<CInput>()->right = false;
						}
					break;
				}
			}
		}
	}

private:
	void handleChase(std::shared_ptr<Entity>& e, const BCondition& bc) {
		switch (bc.trigger) {
			case TR::PLAYER_NEARBY:
			{
				if (CheckCollision::of(e, play_data.player, bc.data_start)) {
					e->get<CBChase>()->target = play_data.player;
				}
				// lose agro if target got too far
				else if (!CheckCollision::of(e, play_data.player, bc.data_stop)) {
					if (e->get<CBChase>()->target = play_data.player) {
						e->get<CBChase>()->target = nullptr;
						e->get<CBChase>()->path.clear();
					}
				}
			}
			break;
			case TR::BASE_NEARBY:
				if (CheckCollision::of(e, play_data.base, bc.data_start)) {
					e->get<CBChase>()->target = play_data.base;
				}
				// lose agro if target got too far
				else if (!CheckCollision::of(e, play_data.base, bc.data_stop)) {
					if (e->get<CBChase>()->target == play_data.base) {
						e->get<CBChase>()->target = nullptr;
						e->get<CBChase>()->path.clear();
					}
				}
			break;
			case TR::BASE_NOT_PROTECTED:
				if (!CheckCollision::of(play_data.player, play_data.base, bc.data_start)) {
					e->get<CBChase>()->target = play_data.base;
				}
				// lose agro if target got too far
				else if (CheckCollision::of(play_data.player, play_data.base, bc.data_stop)) {
					if (e->get<CBChase>()->target == play_data.base) {
						e->get<CBChase>()->target = nullptr;
						e->get<CBChase>()->path.clear();
					}
				}
			break;
			case TR::BASE_LOW_HP:
				if (play_data.base->get<CStats>()->effective[CStats::HEALTH] < play_data.base->get<CStats>()->initial[CStats::HEALTH] * bc.data_start / 100) {
					e->get<CBChase>()->target = play_data.base;
				}
			break;
		}
	}

	void handleFire(std::shared_ptr<Entity>& e, const BCondition& bc, bool& fire_weapon) {
		// MUST CHECK COLLISION BETWEEN PROJECTILE SPAWN POSTION AND TARGET
		// ADD NEW CheckCollision::of() FUNCTION

		e->get<CBFire>()->target = nullptr;

		switch (bc.trigger) {
			case TR::CONTINUOUS:
				fire_weapon = true;
			break;
			case TR::RANDOM:
				if (rand() % 2) fire_weapon = true;
				else fire_weapon = false;
			break;
			case TR::PLAYER_NEARBY:
				if (CheckCollision::of(e, play_data.player, bc.data_start)) {
					fire_weapon = true;
					e->get<CBFire>()->target = play_data.player;
				}
			break;
			case TR::BASE_NEARBY:
				if (CheckCollision::of(e, play_data.base, bc.data_start)) {
					fire_weapon = true;
					e->get<CBFire>()->target = play_data.base;
				}
			break;
		}

		if (e->get<CBChase>()) {
			if (e->get<CBChase>()->target) {
				e->get<CBFire>()->target = e->get<CBChase>()->target;
			}
		}

		if (e->get<CBFire>()->target) {
			e->facing = facingOf(e->get<CBFire>()->target->get<CTransform>()->pos - e->get<CTransform>()->pos);
		}
	}

	size_t facingOf(sf::Vector2f v) {
		float ang = angle(v);

		if (ang >= 0 && ang < 22.5) return Entity::FACING_E;
		else if (ang >= 337.5 && ang < 360) return Entity::FACING_E;
		else if (ang >= 22.5 && ang < 67.5) return Entity::FACING_NE;
		else if (ang >= 67.5 && ang < 112.5) return Entity::FACING_N;
		else if (ang >= 112.5 && ang < 157.5) return Entity::FACING_NW;
		else if (ang >= 112.5 && ang < 202.5) return Entity::FACING_W;
		else if (ang >= 202.5 && ang < 247.5) return Entity::FACING_SW;
		else if (ang >= 247.5 && ang < 292.5) return Entity::FACING_S;
		else if (ang >= 292.5 && ang < 337.5) return Entity::FACING_SE;

		return 0;
	}


	void lookAt(CInput& c_input, const sf::Vector2f& a, const sf::Vector2f& b) {
		size_t facing = facingOf(a-b);
		sf::Vector2f dir = dirOf(facing);

		// reset direction
		c_input.right = false;
		c_input.left = false;
		c_input.up = false;
		c_input.down = false;

		// set new direction
		if (dir.x > 0) c_input.right = true;
		else if (dir.x < 0) c_input.left = true;
		if (dir.y > 0) c_input.down = true;
		else if (dir.y < 0) c_input.up = true;
	}


	float angle(sf::Vector2f v) {
		v.y*=-1;
		float ang = atan(v.y/v.x);

		if (v.x < 0 && v.y > 0) ang += PI;
		else if (v.x < 0 && v.y < 0) ang += PI;
		else if (v.x > 0 && v.y < 0) ang += 2*PI;

		return ang / PI * 180;
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