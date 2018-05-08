#pragma once
#include "Event.h"
class MoveEvent :
	public Event
{
public:
	enum Direction
	{
		Left,
		Up,
		Down,
		Right
	};

	MoveEvent(Direction direction, __int64 dt);
	~MoveEvent();

	Direction getDirection() const { return _direction; }
	__int64 getDt() const { return _dt; }

private:
	Direction _direction;
	__int64 _dt;
};

