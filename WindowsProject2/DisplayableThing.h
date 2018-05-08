#pragma once
#include "Displayable.h"
#include "Controllable.h"
class Possessor;

class DisplayableThing :
	public Displayable,
	public Controllable
{
public:
	DisplayableThing(float x, float y, float width, float height);
	~DisplayableThing();

	void update(__int64 dt);
	DrawType getDrawType() const { return Rectangle; }

	// these are the controlls that the possesor will use
	void moveLeft(__int64 dt);
	void moveUp(__int64 dt);
	void moveRight(__int64 dt);
	void moveDown(__int64 dt);

	// the possessor will be keyboard input or AI
	void setPossesor(Possessor* possessor);

private:
	Possessor* _possessor;
};

