#pragma once

class GameObject;

// base class for AI or keyboard input
class Possessor 
{
public:
	Possessor();
	~Possessor();

	void setControllable(GameObject* controller) { _controller = controller; }
	virtual void update(__int64 dt) {}

protected:
	GameObject * _controller;
};

