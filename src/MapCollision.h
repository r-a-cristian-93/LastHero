/*
Copyright © 2011-2012 Clint Bellanger
Copyright © 2013-2016 Justin Jacobs

This file is part of FLARE.

FLARE is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

FLARE is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
FLARE.  If not, see http://www.gnu.org/licenses/
*/

/*
 * MapCollision.h
 * RPGEngine
 *
 * Handle collisions between objects and the map
 */

#ifndef MAP_COLLISION_H
#define MAP_COLLISION_H

#include "Common.h"
#include "EntityManager.h"

typedef std::vector< std::vector<unsigned short> > CollisionLayer;

class MapCollision {
private:
	static const float MIN_TILE_GAP;

	// collision check types
	enum {
		CHECK_MOVEMENT = 1,
		CHECK_SIGHT = 2
	};

	bool isTileOutsideMap(const int& tile_x, const int& tile_y) const;

	bool lineCheck(const float& x1, const float& y1, const float& x2, const float& y2, int check_type, int movement_type);

	bool smallStepForcedSlideAlongGrid(
		float &x, float &y, float step_x, float step_y, int movement_type, int collide_type);
	bool smallStepForcedSlide(
		float &x, float &y, float step_x, float step_y, int movement_type, int collide_type);
	bool smallStep(
		float &x, float &y, float step_x, float step_y, int movement_type, int collide_type);

	bool isValidTile(const int& x, const int& y, int movement_type, int collide_type) const;

	sf::Vector2f collisionToMap(const sf::Vector2i& p);

	void setCell(sf::Vector2i m_pos, int blocks_type);

public:
	// const flags
	static const bool IGNORE_BLOCKED = true;
	static const bool IS_ALLY = true;
	static const int DEFAULT_PATH_LIMIT = 0;

	// entity collision type
	enum {
		COLLIDE_NORMAL = 0,
		COLLIDE_HERO = 1,
		COLLIDE_NO_ENTITY = 2
	};

	// movement options
	enum {
		MOVE_NORMAL = 0,
		MOVE_FLYING = 1, // can move through BLOCKS_MOVEMENT (e.g. water)
		MOVE_INTANGIBLE = 2 // can move through BLOCKS_ALL (e.g. walls)
	};

	// collision tile types
	// The numbers 0..6 are the collision tiles as produced by tiled,
	// only 7 and 8 deal with entities on the map
	enum {
		BLOCKS_NONE = 0,
		BLOCKS_ALL = 1,
		BLOCKS_MOVEMENT = 2,
		BLOCKS_ALL_HIDDEN = 3,
		BLOCKS_MOVEMENT_HIDDEN = 4,
		MAP_ONLY = 5,
		MAP_ONLY_ALT = 6,
		BLOCKS_ENTITIES = 7, // hero or enemies are blocking this tile, so any other entity is blocked
		BLOCKS_ENEMIES = 8  // an ally is standing on that tile, so the hero could pass if ENABLE_ALLY_COLLISION is false
	};

	MapCollision(EntityManager& _ent_mgr);
	~MapCollision();
	float calcDist(const sf::Vector2f& p1, const sf::Vector2f& p2);

	void setMap(sf::Vector2u _map_size, sf::Vector2u _tile_size, unsigned short _resolution, unsigned short _update);
	bool move(float &x, float &y, float step_x, float step_y, int movement_type, int collide_type);

	bool isOutsideMap(const float& tile_x, const float& tile_y) const;
	bool isEmpty(const float& x, const float& y) const;
	bool isWall(const float& x, const float& y) const;

	bool isValidPosition(const float& x, const float& y, int movement_type, int collide_type) const;

	bool lineOfSight(const float& x1, const float& y1, const float& x2, const float& y2);

	bool isFacing(const float& x1, const float& y1, char direction, const float& x2, const float& y2);

	bool computePath(const sf::Vector2f& start, const sf::Vector2f& end, std::vector<sf::Vector2f> &path, int movement_type, unsigned int limit);

	sf::Vector2f getRandomNeighbor(const sf::Vector2i& target, int range, bool ignore_blocked);

	int getCollideType(bool hero) {
		return hero ? COLLIDE_HERO : COLLIDE_NORMAL;
	}

	void update();

	CollisionLayer colmap;			// numbers of occupants
	CollisionLayer colmap_r;		// radius of hitbox
	sf::Vector2u map_size;
	sf::Vector2u tile_size;
	unsigned short resolution;
	EntityManager& ent_mgr;

	unsigned short update_interval;
	unsigned short update_frame;
};

#endif
