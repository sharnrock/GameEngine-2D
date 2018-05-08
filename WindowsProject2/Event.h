#pragma once
class Event
{
public:
	enum Type
	{
		Move,
		Collision
	};

	Event();
	virtual ~Event();

	virtual Type getType() const = 0;
};

