#include <fstream>
#include <iostream>
#include <cmath>

#include "Game.h"
#include "SDraw.h"
#include "SUpdate.h"
#include "Assets.h"

float squareDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}


Game::Game(std::string file_name)
	:score(0)
	,running(true)
	,paused(false)
	,replay(false)
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

			if (replay) {
				sPlayback();
			}

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

	Assets res;
	res.loadEntities();

	while(file >> word) {
		if (word == "Window") {
			file >> app_conf.window_name;
			file >> app_conf.window_w;
			file >> app_conf.window_h;
			file >> app_conf.max_fps;
			int style_bits;
			file >> style_bits;
			app_conf.window_style = 1 << style_bits;
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

	window.create(sf::VideoMode(app_conf.window_w, app_conf.window_h), app_conf.window_name, app_conf.window_style);
	window.setFramerateLimit(app_conf.max_fps);
	window.setKeyRepeatEnabled(false);

	score_text = sf::Text("0", font, app_conf.font_size);
	score_text.setFillColor(sf::Color(app_conf.font_r, app_conf.font_g, app_conf.font_b));
	score_text.setPosition(20.0f, 20.0f);

	ent_mgr = EntityManager();
	act_mgr = ActionManager();

	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::W, Action::MOVE_UP);
	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::A, Action::MOVE_LEFT);
	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::S, Action::MOVE_DOWN);
	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::D, Action::MOVE_RIGHT);
	act_mgr.registerAction(ActionManager::DEV_MOUSE, sf::Mouse::Left, Action::FIRE_PRIMARY);
	act_mgr.registerAction(ActionManager::DEV_MOUSE, sf::Mouse::Right, Action::FIRE_SECONDARY);

	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::P, Action::GAME_PAUSE);
	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::F1, Action::REPLAY_SAVE);
	act_mgr.registerAction(ActionManager::DEV_KEYBOARD, sf::Keyboard::F2, Action::REPLAY_START);

	spawnPlayer();
}

void Game::spawnPlayer() {
	const sf::Vector2f pos(app_conf.window_w/2, app_conf.window_h/2);

	std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(p_conf.shape_radius, p_conf.vertices);
	shape->setOrigin(p_conf.shape_radius, p_conf.shape_radius);
	shape->setFillColor(sf::Color(p_conf.fill_r, p_conf.fill_g, p_conf.fill_b));
	shape->setOutlineColor(sf::Color(p_conf.out_r, p_conf.out_g, p_conf.out_b));
	shape->setOutlineThickness(p_conf.out_thk);
	shape->setPosition(pos);

	player = ent_mgr.add(Entity::TAG_PLAYER);

	player->add<CTransform>(new CTransform(pos, p_conf.velocity));
	player->get<CTransform>()->d_angle = 2;
	player->add<CShape>(new CShape(shape));
	player->add<CCollision>(new CCollision(p_conf.collision_radius));
	player->add<CInput>(new CInput());
	player->alive = true;
}

void Game::spawnEnemy() {
	const size_t vertices = static_cast<size_t>(rand() % (e_conf.vertices_max - e_conf.vertices_min) + e_conf.vertices_min);
	const float vel = rand() % (e_conf.velocity_max - e_conf.velocity_min) + e_conf.velocity_min;
	const sf::Vector2f dir(rand(), rand());
	const int r = rand() % 255;
	const int g = rand() % 200 + 55;
	const int b = rand() % 55 + 100;
	bool position_is_valid = false;
	sf::Vector2f pos;

	while (!position_is_valid) {
		pos.x = rand() % static_cast<int>(app_conf.window_w - e_conf.shape_radius*2) + e_conf.shape_radius;
		pos.y = rand() % static_cast<int>(app_conf.window_h - e_conf.shape_radius*2) + e_conf.shape_radius;

		float square_min_dist = (p_conf.shape_radius*10 + e_conf.shape_radius) * (p_conf.shape_radius*10 + e_conf.shape_radius);
		float square_current_dist = squareDistance(pos, player->get<CTransform>()->pos);

		if (square_current_dist > square_min_dist) {
			position_is_valid = true;
		}
	}

	std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(e_conf.shape_radius, vertices);
	shape->setOrigin(e_conf.shape_radius, e_conf.shape_radius);
	shape->setFillColor(sf::Color(r, g, b));
	shape->setOutlineColor(sf::Color(e_conf.out_r, e_conf.out_g, e_conf.out_b));
	shape->setOutlineThickness(e_conf.out_thk);
	shape->setPosition(pos);

	std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_ENEMY);

	e->add<CTransform>(new CTransform(pos, dir, vel));
	e->get<CTransform>()->d_angle = vel;
	e->add<CShape>(new CShape(shape));
	e->add<CCollision>(new CCollision(e_conf.collision_radius));
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
	Action* action = nullptr;
	int action_code(Action::NONE);

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			running = false;
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			action_code = act_mgr.getCode(ActionManager::DEV_KEYBOARD, event.key.code);

			if (action_code) {
				action = new Action(action_code, Action::TYPE_START, frame_current);
				if (!replay && action_code < Action::GAME_PAUSE) {
					rpl_mgr.log(action);
				}
				doAction(action);
			}
		}
		else if (event.type == sf::Event::KeyReleased) {
			action_code = act_mgr.getCode(ActionManager::DEV_KEYBOARD, event.key.code);

			if (action_code) {
				action = new Action(action_code, Action::TYPE_END, frame_current);
				if (!replay && action_code < Action::GAME_PAUSE) {
					rpl_mgr.log(action);
				}
				doAction(action);
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed && !paused){
			action_code = act_mgr.getCode(ActionManager::DEV_MOUSE, event.mouseButton.button);

			if (action_code != 0) {
				action = new Action(action_code, Action::TYPE_START, sf::Mouse::getPosition(), frame_current);
				if (!replay && action_code < Action::GAME_PAUSE) {
					rpl_mgr.log(action);
				}
				doAction(action);
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
	const size_t vertices = parent->get<CShape>()->shape->getPointCount();
	const float rotation = parent->get<CShape>()->shape->getRotation();
	const float alpha = 360 / vertices;
	const float radius = parent->get<CShape>()->shape->getRadius() / vertices;
	const float vel = parent->get<CTransform>()->max_velocity / 2;
	const sf::Color fill_color = parent->get<CShape>()->shape->getFillColor();
	sf::Vector2f dir;
	sf::Vector2f pos;

	for (size_t i=0; i<vertices; i++) {
		dir.x = cos((alpha*i + rotation) * PI / 180);
		dir.y = sin((alpha*i + rotation) * PI / 180);
		pos = parent->get<CTransform>()->pos + dir*(radius*2);

		std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(radius, vertices);
		shape->setOrigin(radius, radius);
		shape->setFillColor(fill_color);
		shape->setPosition(pos);

		std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_CHILD);

		e->add<CTransform>(new CTransform(pos, dir, vel));
		e->get<CTransform>()->d_angle = vel;
		e->add<CShape>(new CShape(shape));
		e->add<CCollision>(new CCollision(radius));
		e->add<CLifespan>(new CLifespan(e_conf.child_lifespan));
		e->alive = true;
	}
}

void Game::sCombat() {
	if (player->get<CInput>()->fire_primary) {
		spawnBullet();
		player->get<CInput>()->fire_primary = false;
	}
	if (player->get<CInput>()->fire_secondary) {
		if (ent_mgr.getEntities(Entity::TAG_MISSLE).size() == 0) {
			spawnMissle();
		}
		player->get<CInput>()->fire_secondary = false;
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
	sf::Vector2f mouse_pos;

	if (replay) {
		mouse_pos = rpl_mgr.nextAction->location;
	}
	else {
		mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));
	}


	const sf::Vector2f pos(player->get<CTransform>()->pos);
	const sf::Vector2f dir = mouse_pos - pos;

	std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(b_conf.shape_radius, b_conf.vertices);
	shape->setOrigin(b_conf.shape_radius, b_conf.shape_radius);
	shape->setFillColor(sf::Color(b_conf.fill_r, b_conf.fill_g, b_conf.fill_b));
	shape->setPosition(pos);

	std::shared_ptr<Entity> bullet = ent_mgr.add(Entity::TAG_BULLET);

	bullet->add<CTransform>(new CTransform(pos, dir, b_conf.velocity));
	bullet->get<CTransform>()->d_angle = 1;
	bullet->add<CShape>(new CShape(shape));
	bullet->add<CCollision>(new CCollision(b_conf.collision_radius));
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
	sf::Vector2f mouse_pos;

	if (replay) {
		mouse_pos = rpl_mgr.nextAction->location;
	}
	else {
		mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));
	}

	const sf::Vector2f pos(player->get<CTransform>()->pos);
	const sf::Vector2f dir = mouse_pos - pos;

	std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(m_conf.shape_radius, m_conf.vertices);
	shape->setOrigin(m_conf.shape_radius, m_conf.shape_radius);
	shape->setFillColor(sf::Color(m_conf.fill_r, m_conf.fill_g, m_conf.fill_b));
	shape->setPosition(pos);

	std::shared_ptr<Entity> missle = ent_mgr.add(Entity::TAG_MISSLE);

	missle->add<CTransform>(new CTransform(pos, dir, m_conf.velocity));
	missle->add<CShape>(new CShape(shape));
	missle->add<CCollision>(new CCollision(m_conf.collision_radius));
	missle->add<CLifespan>(new CLifespan(m_conf.lifespan));
	missle->add<CTarget>(new CTarget());
	missle->alive = true;
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

void Game::doAction(const Action* a) {
	if (a->type == Action::TYPE_START) {
		switch (a->code) {
			case Action::MOVE_UP:
				player->get<CInput>()->up = true;
			break;
			case Action::MOVE_LEFT:
				player->get<CInput>()->left = true;
			break;
			case Action::MOVE_DOWN:
				player->get<CInput>()->down = true;
			break;
			case Action::MOVE_RIGHT:
				player->get<CInput>()->right = true;
			break;
			case Action::FIRE_PRIMARY:
				player->get<CInput>()->fire_primary = true;
			break;
			case Action::FIRE_SECONDARY:
				player->get<CInput>()->fire_secondary = true;
			break;
			case Action::GAME_PAUSE:
				paused = !paused;
			break;
			case Action::REPLAY_SAVE:
				rpl_mgr.save();
			break;
			case Action::REPLAY_START:
				replay = true;
				rpl_mgr.start();
				frame_current = 0;
			break;
			default:
			break;
		}
	}
	if (a->type == Action::TYPE_END) {
		switch (a->code) {
			case Action::MOVE_UP:
				player->get<CInput>()->up = false;
			break;
			case Action::MOVE_LEFT:
				player->get<CInput>()->left = false;
			break;
			case Action::MOVE_DOWN:
				player->get<CInput>()->down = false;
			break;
			case Action::MOVE_RIGHT:
				player->get<CInput>()->right = false;
			break;
			default:
			break;
		}
	}
}

void Game::sPlayback() {

	std::cout << rpl_mgr.cursor << " " << rpl_mgr.stream.actions.size() << std::endl;
	if (rpl_mgr.cursor >= rpl_mgr.stream.actions.size() - 1) {
		replay = false;
		std::cout << "PLAYBACK STOPPPED\n";
	}

	if (rpl_mgr.nextAction->frame == frame_current) {
		doAction(rpl_mgr.nextAction);
		rpl_mgr.next();
	}
}
