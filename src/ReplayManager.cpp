#include <fstream>

#include "ReplayManager.h"

ReplayManager::ReplayManager()
	:stream()
	,frame(0)
	,cursor(0)
	{}


void ReplayManager::log(Action*& a) {
	std::cout << "LOG\n";
	stream << a;
}

void ReplayManager::next() {
	cursor++;
	nextAction = &stream[cursor];
}


void ReplayManager::start() {
	load();
	cursor = 0;
	frame = 0;
	nextAction = &stream[0];
}

void ReplayManager::load() {
	std::ifstream file("replay.txt");

	size_t code;
	size_t type;
	int frame;
	int x, y;
	Action* action;

	stream.clear();

	while (file >> code) {
		file >> type >> frame >> x >> y;
		action = new Action(code, type, sf::Vector2i(x,y), frame);
		stream << action;
	}

	file.close();
}

void ReplayManager::save() {
	std::remove("repaly.txt");	
	std::ofstream file("replay.txt");

	Action* action = nullptr;

	while (!stream.empty()) {
		stream >> action;
		file << action << std::endl;
	}

	file.close();
}
