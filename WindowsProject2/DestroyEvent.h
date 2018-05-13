#pragma once

#include "Event.h"

class DestroyEvent :
	public Event
{
public:
	DestroyEvent();
	~DestroyEvent();
};

