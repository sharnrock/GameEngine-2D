#pragma once
class Event
{
public:
	enum Type
	{
		Collision,
		Birth,
		Destroy,
		Animation,
		Control
	};

	Event();
	virtual ~Event();

	virtual Type getType() const = 0;
};
