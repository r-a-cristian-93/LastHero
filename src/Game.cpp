#include <fstream>
#include <iostream>
#include <cmath>

#include "Game.h"
#include "SDraw.h"
#include "SUpdate.h"

float squareDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}


Game::Game(std::string file_name)
	:score(0)
	,running(true)
	,paused(false)
	,frame_current(0)
	,frame_last_spawn(0)
{
	init(file_name);
}

void Game::run() {
	sf::Event event;

	sf::FloatRect lim(0,0,app_conf.window_w,app_conf.window_h);

	while(running) {
		if (window.isOpen()) {

			window.clear();
			if (!paused) {
				ent_mgr.update();
				sEnemySpawner();
				sLifespan();
				sMissleGuidance();
				SUpdate::updatePosition(ent_mgr.getEntities(), lim);
				sCollision();
				sCombat();
			}

			sSpin();
			SDraw::drawEntities(&window, ent_mgr.getEntities());
			sUserInput();

			std::string sc = std::to_string(score);
			score_text.setString(sc);

			window.draw(score_text);
			window.display();
		}
		frame_current++;
	}
}

void Game::init(std::string file_name) {
	std::ifstream file(file_name);
	std::string word;

	while(file >> word) {
		if (word == "Window") {
			file >> app_conf.window_name;
			file >> app_conf.window_w;
			file >> app_conf.window_h;
			file >> app_conf.max_fps;
			file >> app_conf.fullscreen;
		}
		if (word == "Font") {
			std::string font_file_path;
			file >> font_file_path;
			file >> app_conf.font_size;
			file >> app_conf.font_r;
			file >> app_conf.font_g;
			file >> app_conf.font_b;
			if (!font.loadFromFile(font_file_path)) {
				std::cerr << "Could not load font!\n";
				exit(-1);
			}
		}
		if (word == "Player") {
			file >> p_conf.shape_radius;
			file >> p_conf.collision_radius;
			file >> p_conf.velocity;
			file >> p_conf.fill_r;
			file >> p_conf.fill_g;
			file >> p_conf.fill_b;
			file >> p_conf.out_r;
			file >> p_conf.out_g;
			file >> p_conf.out_b;
			file >> p_conf.out_thk;
			file >> p_conf.vertices;
		}
		if (word == "Enemy") {
			file >> e_conf.shape_radius;
			file >> e_conf.collision_radius;
			file >> e_conf.velocity_min;
			file >> e_conf.velocity_max;
			file >> e_conf.out_r;
			file >> e_conf.out_g;
			file >> e_conf.out_b;
			file >> e_conf.out_thk;
			file >> e_conf.vertices_min;
			file >> e_conf.vertices_max;
			file >> e_conf.child_lifespan;
			file >> e_conf.spawn_interval;
		}
		if (word == "Bullet") {
			file >> b_conf.shape_radius;
			file >> b_conf.collision_radius;
			file >> b_conf.velocity;
			file >> b_conf.fill_r;
			file >> b_conf.fill_g;
			file >> b_conf.fill_b;
			file >> b_conf.out_r;
			file >> b_conf.out_g;
			file >> b_conf.out_b;
			file >> b_conf.out_thk;
			file >> b_conf.vertices;
			file >> b_conf.lifespan;
		}
		if (word == "Missle") {
			file >> m_conf.shape_radius;
			file >> m_conf.collision_radius;
			file >> m_conf.velocity;
			file >> m_conf.fill_r;
			file >> m_conf.fill_g;
			file >> m_conf.fill_b;
			file >> m_conf.vertices;
			file >> m_conf.cooldown;
			file >> m_conf.lifespan;
		}
	}

	file.close();

	window.create(sf::VideoMode(app_conf.window_w, app_conf.window_h), app_conf.window_name, sf::Style::Fullscreen);

	window.setFramerateLimit(app_conf.max_fps);
	window.setKeyRepeatEnabled(false);

	ent_mgr = EntityManager();

	score_text = sf::Text("0", font, app_conf.font_size);
	score_text.setFillColor(sf::Color(app_conf.font_r, app_conf.font_g, app_conf.font_b));
	score_text.setPosition(20.0f, 20.0f);

	spawnPlayer();
}

void Game::spawnPlayer() {
	player = ent_mgr.add(Entity::TAG_PLAYER);

	std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(p_conf.shape_radius, p_conf.vertices);
	player->add<CShape>(new CShape(shape));
	shape->setOrigin(p_conf.shape_radius, p_conf.shape_radius);
	shape->setFillColor(sf::Color(p_conf.fill_r, p_conf.fill_g, p_conf.fill_b));
	shape->setOutlineColor(sf::Color(p_conf.out_r, p_conf.out_g, p_conf.out_b));
	shape->setOutlineThickness(p_conf.out_thk);

	player->add<CCollision>(new CCollision(p_conf.collision_radius));

	sf::Vector2f pos(app_conf.window_w/2, app_conf.window_h/2);
	player->add<CTransform>(new CTransform(pos, p_conf.velocity));
	player->get<CTransform>()->d_angle = 2;

	player->get<CShape>()->shape->setPosition(player->get<CTransform>()->pos);

	player->alive = true;

	player->add<CInput>(new CInput());
}

void Game::spawnEnemy() {
	std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_ENEMY);

	size_t vertices = static_cast<size_t>(rand() % (e_conf.vertices_max - e_conf.vertices_min) + e_conf.vertices_min);
	std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(e_conf.shape_radius, vertices);

	e->add<CShape>(new CShape(shape));
	shape->setOrigin(e_conf.shape_radius, e_conf.shape_radius);

	int r = rand() % 255;
	int g = rand() % 200 + 55;
	int b = rand() % 55 + 100;
	shape->setFillColor(sf::Color(r, g, b));
	shape->setOutlineColor(sf::Color(e_conf.out_r, e_conf.out_g, e_conf.out_b));
	shape->setOutlineThickness(e_conf.out_thk);

	e->add<CCollision>(new CCollision(e_conf.collision_radius));

	bool position_is_set = false;
	sf::Vector2f pos;

	while (!position_is_set) {
		pos.x = rand() % static_cast<int>(app_conf.window_w - e_conf.shape_radius*2) + e_conf.shape_radius;
		pos.y = rand() % static_cast<int>(app_conf.window_h - e_conf.shape_radius*2) + e_conf.shape_radius;

		float square_min_dist = (p_conf.shape_radius*10 + e_conf.shape_radius) * (p_conf.shape_radius*10 + e_conf.shape_radius);
		float square_current_dist = squareDistance(pos, player->get<CTransform>()->pos);

		if (square_current_dist > square_min_dist) {
			position_is_set = true;
		}
	}

	float velocity = rand() % (e_conf.velocity_max - e_conf.velocity_min) + e_conf.velocity_min;
	e->add<CTransform>(new CTransform(pos, velocity));
	e->get<CTransform>()->d_angle = rand() % e_conf.velocity_max + e_conf.velocity_min;
	e->get<CTransform>()->dir = {1,1};
	e->get<CShape>()->shape->setPosition(e->get<CTransform>()->pos);

	e->add<CScore>(new CScore(vertices));

	e->alive = true;
}

void Game::sEnemySpawner() {
	if (frame_current-frame_last_spawn >= e_conf.spawn_interval) {
		spawnEnemy();

		frame_last_spawn = frame_current;
	}
}

void Game::sUserInput() {

	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			running = false;
			window.close();
		}

		if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
				case sf::Keyboard::W :
					player->get<CInput>()->up = true;
					break;
				case sf::Keyboard::S :
					player->get<CInput>()->down = true;
					break;
				case sf::Keyboard::A :
					player->get<CInput>()->left = true;
					break;
				case sf::Keyboard::D :
					player->get<CInput>()->right = true;
					break;
				case sf::Keyboard::Space:
					paused = !paused;
					break;
			}
		}
		if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code) {
				case sf::Keyboard::W :
					player->get<CInput>()->up = false;
					break;
				case sf::Keyboard::S :
					player->get<CInput>()->down = false;
					break;
				case sf::Keyboard::A :
					player->get<CInput>()->left = false;
					break;
				case sf::Keyboard::D :
					player->get<CInput>()->right = false;
				break;
			}
		}

		if (!paused) {
			if (event.type == sf::Event::MouseButtonPressed){
				if (event.mouseButton.button == sf::Mouse::Left) {
					player->get<CInput>()->shoot = true;
				}
			}
			if (event.type == sf::Event::MouseButtonPressed){
				if (event.mouseButton.button == sf::Mouse::Right) {
					player->get<CInput>()->fire_missle = true;
				}
			}
		}
	}
}

bool checkCollision(std::shared_ptr<Entity>& a, std::shared_ptr<Entity>& b) {
	float square_distance = squareDistance(a->get<CTransform>()->pos, b->get<CTransform>()->pos);

	if (a->get<CCollision>() && b->get<CCollision>()) {
		int square_radius =
			(a->get<CCollision>()->radius + b->get<CCollision>()->radius) *
			(a->get<CCollision>()->radius + b->get<CCollision>()->radius);

		if (square_distance < square_radius) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}


void Game::sCollision() {
	bool collision = false;

	//what kills player
	for (std::shared_ptr<Entity>& enemy : ent_mgr.getEntities(Entity::TAG_ENEMY)) {
		collision = checkCollision(player, enemy);
		if (collision) {
			player->alive = false;
			score /= 2;
			spawnPlayer();
		}
	}

	for (std::shared_ptr<Entity>& child : ent_mgr.getEntities(Entity::TAG_CHILD)) {
		collision = checkCollision(player, child);
		if (collision) {
			player->alive = false;
			score /= 2;
			spawnPlayer();
		}
	}

	//what kills enemies
	for (std::shared_ptr<Entity>& bullet : ent_mgr.getEntities(Entity::TAG_BULLET)) {
		for (std::shared_ptr<Entity>& enemy : ent_mgr.getEntities(Entity::TAG_ENEMY)) {
			collision = checkCollision(bullet, enemy);
			if (collision) {
				enemy->alive = false;
				bullet->alive = false;
				spawnChilds(enemy);

				if (enemy->get<CScore>()) {
					score += enemy->get<CScore>()->score;
				}

			}
		}

		for (std::shared_ptr<Entity>& child : ent_mgr.getEntities(Entity::TAG_CHILD)) {
			collision = checkCollision(bullet, child);
			if (collision) {
				child->alive = false;
				bullet->alive = false;

				if (child->get<CScore>()) {
					score += child->get<CScore>()->score;
				}
			}
		}
	}

	for (std::shared_ptr<Entity>& missle : ent_mgr.getEntities(Entity::TAG_MISSLE)) {
		for (std::shared_ptr<Entity>& enemy : ent_mgr.getEntities(Entity::TAG_ENEMY)) {
			collision = checkCollision(missle, enemy);
			if (collision) {
				enemy->alive = false;
				missle->alive = false;

				if (enemy->get<CScore>()) {
					score += enemy->get<CScore>()->score;
				}
			}
		}

		for (std::shared_ptr<Entity>& child : ent_mgr.getEntities(Entity::TAG_CHILD)) {
			collision = checkCollision(missle, child);
			if (collision) {
				child->alive = false;

				if (child->get<CScore>()) {
					score += child->get<CScore>()->score;
				}
			}
		}
	}
}

void Game::spawnChilds(const std::shared_ptr<Entity>& parent) {
	size_t vertices = parent->get<CShape>()->shape->getPointCount();
	float rotation = parent->get<CShape>()->shape->getRotation();
	float alpha = 360 / vertices;
	sf::Vector2f parent_pos = parent->get<CTransform>()->pos;
	sf::Color fill_color = parent->get<CShape>()->shape->getFillColor();
	float radius = parent->get<CShape>()->shape->getRadius() / vertices;
	float max_velocity = parent->get<CTransform>()->max_velocity / 2;

	for (size_t i=0; i<vertices; i++) {

		std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_CHILD);

		std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(radius, vertices);
		e->add<CShape>(new CShape(shape));
		shape->setOrigin(radius, radius);
		shape->setFillColor(fill_color);

		e->add<CCollision>(new CCollision(radius));

		sf::Vector2f dir;
		dir.x = cos((alpha*i + rotation) * PI / 180);
		dir.y = sin((alpha*i + rotation) * PI / 180);

		e->add<CTransform>(new CTransform(parent_pos+ dir*(radius*2), max_velocity));
		e->get<CTransform>()->d_angle = max_velocity;
		e->get<CTransform>()->dir = dir;
		e->get<CShape>()->shape->setPosition(e->get<CTransform>()->pos);

		e->add<CLifespan>(new CLifespan(e_conf.child_lifespan));

		e->alive = true;
	}
}

void Game::sCombat() {
	if (player->get<CInput>()->shoot) {
		spawnBullet();
		player->get<CInput>()->shoot = false;
	}
	if (player->get<CInput>()->fire_missle) {
		if (ent_mgr.getEntities(Entity::TAG_MISSLE).size() == 0) {
			spawnMissle();
		}
		player->get<CInput>()->fire_missle = false;
	}
}

void Game::sLifespan() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities(Entity::TAG_BULLET)) {
		checkLifespan(e);
	}

	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities(Entity::TAG_CHILD)) {
		checkLifespan(e);
	}
}

void Game::checkLifespan(std::shared_ptr<Entity>& e) {
	if (e->get<CLifespan>()) {
		const int lifespan = e->get<CLifespan>()->lifespan;
		int& remaining = e->get<CLifespan>()->remaining;

		remaining--;

		if (remaining * 3 < lifespan) {
			int alpha = remaining * 255 / lifespan*3;

			sf::Color color = e->get<CShape>()->shape->getFillColor();
			color.a = alpha;
			e->get<CShape>()->shape->setFillColor(color);
		}

		if (remaining <= 0) {
			e->alive = false;
		}
	}
}

void Game::spawnBullet() {
	sf::Vector2f mouse_pos(sf::Mouse::getPosition(window));
	sf::Vector2f player_pos(player->get<CTransform>()->pos);
	sf::Vector2f direction = mouse_pos - player_pos;

	std::shared_ptr<Entity> bullet = ent_mgr.add(Entity::TAG_BULLET);

	std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(b_conf.shape_radius, b_conf.vertices);
	bullet->add<CShape>(new CShape(shape));
	shape->setOrigin(b_conf.shape_radius, b_conf.shape_radius);
	shape->setFillColor(sf::Color(b_conf.fill_r, b_conf.fill_g, b_conf.fill_b));

	bullet->add<CCollision>(new CCollision(b_conf.collision_radius));

	sf::Vector2f pos(player_pos);
	bullet->add<CTransform>(new CTransform(pos, b_conf.velocity));
	bullet->get<CTransform>()->dir = direction;
	bullet->get<CTransform>()->d_angle = 1;
	bullet->get<CShape>()->shape->setPosition(bullet->get<CTransform>()->pos);

	bullet->add<CLifespan>(new CLifespan(b_conf.lifespan));

	bullet->alive = true;
}

void Game::sSpin() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities()) {
		if (e->get<CTransform>()) {
			if (e->get<CTransform>()->d_angle) {
				e->get<CShape>()->shape->rotate(e->get<CTransform>()->d_angle);
			}
		}
	}
}

void Game::spawnMissle() {
	sf::Vector2f mouse_pos(sf::Mouse::getPosition(window));
	sf::Vector2f player_pos(player->get<CTransform>()->pos);

	std::shared_ptr<Entity> missle = ent_mgr.add(Entity::TAG_MISSLE);

	std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(m_conf.shape_radius, m_conf.vertices);
	missle->add<CShape>(new CShape(shape));
	shape->setOrigin(m_conf.shape_radius, m_conf.shape_radius);
	shape->setFillColor(sf::Color(m_conf.fill_r, m_conf.fill_g, m_conf.fill_b));

	missle->add<CCollision>(new CCollision(m_conf.collision_radius));

	missle->add<CTransform>(new CTransform(player_pos, m_conf.velocity));
	missle->get<CTransform>()->dir = mouse_pos - player_pos;

	missle->get<CShape>()->shape->setPosition(missle->get<CTransform>()->pos);

	missle->add<CLifespan>(new CLifespan(m_conf.lifespan));

	missle->alive = true;

	missle->add<CTarget>(new CTarget());
}

std::shared_ptr<Entity> Game::findTarget(const std::shared_ptr<Entity>& missle) {
	EntityVec reachable;
	sf::Vector2f dir_missle(missle->get<CTransform>()->dir);
	sf::Vector2f dir_enemy;

	for (std::shared_ptr<Entity>& enemy : ent_mgr.getEntities(Entity::TAG_ENEMY)) {
		dir_enemy = enemy->get<CTransform>()->pos - missle->get<CTransform>()->pos;

		if (angle(dir_missle, dir_enemy) < 30) {
			reachable.push_back(enemy);
		}
	}

	std::shared_ptr<Entity> target;
	float prev_dist(app_conf.window_w*app_conf.window_w);
	float dist;

	for (std::shared_ptr<Entity>& enemy : reachable) {
		dist = abs(squareDistance(missle->get<CTransform>()->pos, enemy->get<CTransform>()->pos));
		if (dist < prev_dist) {
			target = enemy;
			prev_dist = dist;
		}
	}

	if (target) {
		return target;
	}
	else {
		return nullptr;
	}
}

void Game::sMissleGuidance() {
	for (std::shared_ptr<Entity>& missle : ent_mgr.getEntities(Entity::TAG_MISSLE)) {

		std::shared_ptr<Entity> new_target = findTarget(missle);
		std::shared_ptr<Entity> prev_target = nullptr;
		bool set_new_target = false;

		if (missle->get<CTarget>()->target) {
			prev_target = missle->get<CTarget>()->target;
		}

		if (!new_target) {
			set_new_target = false;
		}
		else {
			if (prev_target) {
				if (prev_target->id == new_target->id) {
					set_new_target = false;
				}
				else {
					set_new_target = true;
				}
			}
			else {
				set_new_target = true;
			}

			if (set_new_target) {
				if (new_target) {
					missle->get<CTarget>()->target = new_target;
				}
			}
		}

		if (set_new_target && missle->get<CTarget>()) {
			if (missle->get<CTarget>()->target) {
				sf::Vector2f target_pos = missle->get<CTarget>()->target->get<CTransform>()->pos;
				sf::Vector2f missle_pos = missle->get<CTransform>()->pos;
				sf::Vector2f dir(target_pos - missle_pos);

				missle->get<CTransform>()->dir = dir;
			}
		}
	}
}

float Game::angle(const sf::Vector2f a, const sf::Vector2f b) {
	float dot_a_b = a.x*b.x + a.y*b.y;
	float mod_a_b = sqrt((a.x*a.x + a.y*a.y) * (b.x*b.x + b.y*b.y));

	return acos(dot_a_b / mod_a_b) / PI * 180;
}
