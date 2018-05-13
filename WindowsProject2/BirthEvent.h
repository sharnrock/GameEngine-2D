#pragma once

#include "Event.h"

class BirthEvent :
	public Event
{
public:
	BirthEvent();
	~BirthEvent();

	Type getType() const { return Event::Birth; }
};

