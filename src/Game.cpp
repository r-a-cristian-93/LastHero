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
	}

	file.close();

	window.create(sf::VideoMode(app_conf.window_w, app_conf.window_h), app_conf.window_name, app_conf.window_style);
	window.setFramerateLimit(app_conf.max_fps);
	window.setKeyRepeatEnabled(false);

	score_text = sf::Text("0", font, app_conf.font_size);
	score_text.setFillColor(sf::Color(app_conf.font_r, app_conf.font_g, app_conf.font_b));
	score_text.setPosition(20.0f, 20.0f);


	assets = new Assets();
	ent_mgr = EntityManager(assets);
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

	player = ent_mgr.add(Entity::TAG_PLAYER);

	player->get<CTransform>()->pos = pos;
	player->get<CShape>()->shape.setPosition(pos);
}

void Game::spawnEnemy() {
	const sf::Vector2f dir(rand(), rand());
	bool position_is_valid = false;
	sf::Vector2f pos;
	int player_radius = player->get<CCollision>()->radius;

	std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_ENEMY, 0);
	int radius = e->get<CCollision>()->radius;

	while (!position_is_valid) {
		pos.x = rand() % static_cast<int>(app_conf.window_w - radius*2) + radius;
		pos.y = rand() % static_cast<int>(app_conf.window_h - radius*2) + radius;

		float square_min_dist = (player_radius*10 + radius) * (player_radius*10 + radius);
		float square_current_dist = squareDistance(pos, player->get<CTransform>()->pos);

		if (square_current_dist > square_min_dist) {
			position_is_valid = true;
		}
	}

	e->get<CTransform>()->pos = pos;
	e->get<CTransform>()->dir = dir;
	e->get<CShape>()->shape.setPosition(pos);
}

void Game::sEnemySpawner() {
	if (frame_current-frame_last_spawn >= 100) {
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
	const size_t vertices = parent->get<CShape>()->shape.getPointCount();
	const float rotation = parent->get<CShape>()->shape.getRotation();
	const float alpha = 360 / vertices;
	const float radius = parent->get<CShape>()->shape.getRadius() / vertices;
	const float vel = parent->get<CTransform>()->max_velocity / 2;
	const int lifespan = static_cast<int>(radius) * 6;
	const sf::Color fill_color = parent->get<CShape>()->shape.getFillColor();
	sf::Vector2f dir;
	sf::Vector2f pos;

	for (size_t i=0; i<vertices; i++) {
		dir.x = cos((alpha*i + rotation) * PI / 180);
		dir.y = sin((alpha*i + rotation) * PI / 180);
		pos = parent->get<CTransform>()->pos + dir*(radius*2);

		CShape* c_shape = new CShape(sf::CircleShape(radius, vertices));
		sf::CircleShape& shape = c_shape->shape;
		shape.setOrigin(radius, radius);
		shape.setFillColor(fill_color);
		shape.setPosition(pos);

		std::shared_ptr<Entity> e = ent_mgr.add(Entity::TAG_CHILD);

		e->add<CTransform>(new CTransform(pos, dir, vel));
		e->get<CTransform>()->d_angle = vel;
		e->add<CShape>(c_shape);
		e->add<CCollision>(new CCollision(radius));
		e->add<CLifespan>(new CLifespan(lifespan));
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

			sf::Color color = e->get<CShape>()->shape.getFillColor();
			color.a = alpha;
			e->get<CShape>()->shape.setFillColor(color);
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

	std::shared_ptr<Entity> bullet = ent_mgr.add(Entity::TAG_BULLET);

	bullet->get<CTransform>()->pos = pos;
	bullet->get<CTransform>()->dir = dir;
	bullet->get<CShape>()->shape.setPosition(pos);
}

void Game::sSpin() {
	for (std::shared_ptr<Entity>& e : ent_mgr.getEntities()) {
		if (e->get<CTransform>()) {
			if (e->get<CTransform>()->d_angle) {
				e->get<CShape>()->shape.rotate(e->get<CTransform>()->d_angle);
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

	std::shared_ptr<Entity> missle = ent_mgr.add(Entity::TAG_MISSLE);

	missle->get<CTransform>()->pos = pos;
	missle->get<CTransform>()->dir = dir;
	missle->get<CShape>()->shape.setPosition(pos);
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
	if (*a->type == Action::TYPE_START) {
		switch (*a->code) {
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
	if (*a->type == Action::TYPE_END) {
		switch (*a->code) {
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

	if (*rpl_mgr.nextAction->frame == frame_current) {
		doAction(rpl_mgr.nextAction);
		rpl_mgr.next();
	}
}
