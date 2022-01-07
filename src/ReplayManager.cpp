#include <fstream>

#include "ReplayManager.h"

ReplayManager::ReplayManager()
	:stream()
	,frame(0)
	{}
	
	
void ReplayManager::log(Action*& a) {
	stream << a;
}
	
	
void ReplayManager::play() {
	//doAction
	
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
	std::ofstream file("replay.txt");
	
	Action* action = nullptr;
	
	while (!stream.empty()) {		
		stream >> action;
		file << action << std::endl;
	}
		
	file.close();
}
