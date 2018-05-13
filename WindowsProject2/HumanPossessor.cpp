#include "HumanPossessor.h"
#include "HumanInputDeviceState.h"

#include "GameObject.h"
#include "ControlEvent.h"


HumanPossessor::HumanPossessor(HumanInputDeviceState* input_devices) :
	_input_devices(input_devices)
{
}

HumanPossessor::~HumanPossessor()
{
}

void HumanPossessor::update(__int64 dt)
{
	if (_input_devices->isPressed('W'))
	{
		_controller->onEvent(&ControlEvent(ControlEvent::MoveUp, dt));
	}
	if (_input_devices->isPressed('S'))
	{
		_controller->onEvent(&ControlEvent(ControlEvent::MoveDown, dt));
	}
	if (_input_devices->isPressed('A'))
	{
		_controller->onEvent(&ControlEvent(ControlEvent::MoveLeft, dt));
	}
	if (_input_devices->isPressed('D'))
	{
		_controller->onEvent(&ControlEvent(ControlEvent::MoveRight, dt));
	}
	if (_input_devices->isPressed(VK_SPACE))
	{
		_controller->onEvent(&ControlEvent(ControlEvent::FirePrimary, dt));
	}
}
