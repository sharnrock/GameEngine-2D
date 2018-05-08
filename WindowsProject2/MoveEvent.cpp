#include "MoveEvent.h"



MoveEvent::MoveEvent(Direction direction, __int64 dt) :
	_direction(direction),
	_dt(dt)
{
}

MoveEvent::~MoveEvent()
{
}
