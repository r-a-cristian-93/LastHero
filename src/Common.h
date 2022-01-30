#ifndef COMMON
#define COMMON

#include <string>
#include <SFML/Graphics.hpp>

#define PI 3.14159265

struct AppConfig {
	std::string window_name;
	int window_w, window_h;
	int max_fps;
	int window_style;
	int font_size;
	int font_r, font_g, font_b;
	float cam_speed, cam_treshold;
};


struct PlayerConfig {
	int velocity;
	float shape_radius;
	int collision_radius;
	int fill_r, fill_g, fill_b;
	int out_r, out_g, out_b, out_thk;
	size_t vertices;
};

struct EnemyConfig {
	int velocity_min, velocity_max;
	float shape_radius;
	int collision_radius;
	int out_r, out_g, out_b, out_thk;
	int child_lifespan;
	size_t vertices_min, vertices_max;
	int spawn_interval;
};

struct BulletConfig {
	int velocity;
	float shape_radius;
	int collision_radius;
	int fill_r, fill_g, fill_b;
	int out_r, out_g, out_b, out_thk;
	int lifespan;
	size_t vertices;
};

struct MissleConfig {
	int velocity;
	float shape_radius;
	int collision_radius;
	int fill_r, fill_g, fill_b;
	size_t vertices;
	int cooldown;
	int lifespan;
};

#endif
