#pragma once

class Controllable;

// base class for AI or keyboard input
class Possessor 
{
public:
	Possessor();
	~Possessor();

	void setControllable(Controllable* controller) { _controller = controller; }
	virtual void update(__int64 dt) {}

protected:
	Controllable * _controller;
};

