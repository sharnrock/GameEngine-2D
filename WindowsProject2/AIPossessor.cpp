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
		auto a = ControlEvent(ControlEvent::MoveLeft, dt);
		_controller->onEvent(&a);
	}
	else
	{
		auto a = ControlEvent(ControlEvent::MoveRight, dt);
		_controller->onEvent(&a);
	}
}
