#pragma once

#include "Event.h"

class AnimationEvent :
	public Event
{
public:
	enum Action
	{
		AnimationLoopEnd
	};

	AnimationEvent(Action action) :
		_action(action)
	{
	}
	~AnimationEvent();

	Type getType() const { return Event::Animation; }
	Action getAction() const { return _action; }

private:
	Action _action;
};

