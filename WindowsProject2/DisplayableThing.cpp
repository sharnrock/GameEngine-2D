#include "DisplayableThing.h"
#include "Possessor.h"
#include <assert.h>


DisplayableThing::DisplayableThing(float x, float y, float width, float height) :
	Displayable(x, y, width, height),
	_possessor(nullptr)
{
}


DisplayableThing::~DisplayableThing()
{
}

void DisplayableThing::update(__int64 dt)
{
	_possessor->update(dt);
}

void DisplayableThing::moveLeft(__int64 dt)
{
	_x += 100.0f * (float)dt / 1000000.0f;
	if (_x > 500)
		_x -= 500;
}

void DisplayableThing::moveUp(__int64 dt)
{

}

void DisplayableThing::moveRight(__int64 dt)
{

}

void DisplayableThing::moveDown(__int64 dt)
{

}

void DisplayableThing::setPossesor(Possessor* possessor)
{ 
	assert(possessor);
	possessor->setControllable(this);
	_possessor = possessor; 
}

