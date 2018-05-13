#pragma once
#include "Event.h"
class ControlEvent :
	public Event
{
public:
	enum Control
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		FirePrimary
	};

	Type getType() const { return Event::Control; }

	ControlEvent(Control dothis, __int64 dt) : 
		_control(dothis),
		_dt(dt)
	{
	}

	~ControlEvent();

	Control getControl() const { return _control; }
	__int64 getDt() const { return _dt; }

private:
	Control _control;
	__int64 _dt;
};

