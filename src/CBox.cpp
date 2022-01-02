#include "CBox.h"

CBox::CBox() 
	:box(sf::FloatRect(0,0,0,0))
	{}
	
CBox::CBox(sf::FloatRect b)
	:box(b)
	{}
