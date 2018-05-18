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
		_dt(dt),
		_mouse_x(0),
		_mouse_y(0)
	{
	}

	~ControlEvent();

	Control getControl() const { return _control; }
	__int64 getDt() const { return _dt; }

	void setMouseCoords(int mouse_x, int mouse_y) { _mouse_x = mouse_x; _mouse_y = mouse_y; }
	int getMouseX() const { return _mouse_x; }
	int getMouseY() const { return _mouse_y; }

private:
	Control _control;
	__int64 _dt;
	int _mouse_x, _mouse_y;
};

