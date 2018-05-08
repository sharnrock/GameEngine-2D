#pragma once

class Possessor;

class Controllable
{
public:
	Controllable();
	virtual ~Controllable();

	virtual void moveLeft(__int64 dt) = 0;
	virtual void moveUp(__int64 dt) = 0;
	virtual void moveRight(__int64 dt) = 0;
	virtual void moveDown(__int64 dt) = 0;

	virtual void setPossesor(Possessor* possessor) = 0;
};

