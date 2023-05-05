#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include <SFML/System.hpp>

class FrameManager {
private:
	sf::Clock clock;

	float frame_dt;
	float frame_increment;

public:
	FrameManager();

	void update();
	const float& getIncrement();
};

#endif