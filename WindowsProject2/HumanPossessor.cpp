#include "HumanPossessor.h"
#include "HumanInputDeviceState.h"
#include "Controllable.h"


HumanPossessor::HumanPossessor(HumanInputDeviceState* input_devices) :
	_input_devices(input_devices)
{
}

HumanPossessor::~HumanPossessor()
{
}

void HumanPossessor::update(__int64 dt)
{
#if 0
	if (_input_devices->isPressed(VK_UP))
		_controller->moveUp(dt);
	if (_input_devices->isPressed(VK_DOWN))
		_controller->moveDown(dt);
	if (_input_devices->isPressed(VK_LEFT))
		_controller->moveLeft(dt);
	if (_input_devices->isPressed(VK_RIGHT))
		_controller->moveRight(dt);
#else
	if (_input_devices->isPressed('W'))
		_controller->moveUp(dt);
	if (_input_devices->isPressed('S'))
		_controller->moveDown(dt);
	if (_input_devices->isPressed('A'))
		_controller->moveLeft(dt);
	if (_input_devices->isPressed('D'))
		_controller->moveRight(dt);
#endif
}
