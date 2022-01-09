#include "CInput.h"

CInput::CInput()
	:up(false)
	,down(false)
	,left(false)
	,right(false)
	,fire_primary(false)
	,fire_secondary(false)
	{}

CInput::CInput(const CInput& c)
	:up(c.up)
	,down(c.down)
	,left(c.left)
	,right(c.right)
	,fire_primary(c.fire_primary)
	,fire_secondary(c.fire_secondary)
	{}
