#include "AIPossessor.h"
#include "Controllable.h"
#include <assert.h>


AIPossessor::AIPossessor()
{
}


AIPossessor::~AIPossessor()
{
}

void AIPossessor::update(__int64 dt)
{
	assert(_controller);

	_dt_total += dt;
	if ((int)(_dt_total /1E6) % 2)
		_controller->moveLeft(dt);
	else 
		_controller->moveRight(dt);
}
