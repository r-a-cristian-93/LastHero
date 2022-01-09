#include "CScore.h"

CScore::CScore()
	:score(0)
	{}

CScore::CScore(const CScore& c)
	:score(c.score)
	{}
	
CScore::CScore(int s)
	:score(s)
	{}
