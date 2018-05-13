#include "AIPossessor.h"
#include <assert.h>
#include "GameObject.h"
#include "ControlEvent.h"

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
	if ((int)(_dt_total / 1E6) % 2)
	{
		_controller->onEvent(&ControlEvent(ControlEvent::MoveLeft, dt));
	}
	else
	{
		_controller->onEvent(&ControlEvent(ControlEvent::MoveRight, dt));
	}
}
