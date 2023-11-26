#include "HumanPossessor.h"
#include "HumanInputDeviceState.h"

#include "GameObject.h"
#include "ControlEvent.h"
#include <string>


HumanPossessor::HumanPossessor(const HumanInputDeviceState* input_devices) :
	_input_devices(input_devices)
{
}

HumanPossessor::~HumanPossessor()
{
}

void HumanPossessor::update(__int64 dt)
{
	assert(_input_devices);

	if (_input_devices->isPressed('W'))
	{
		auto a = ControlEvent(ControlEvent::MoveUp, dt);
		_controller->onEvent(&a);
	}
	if (_input_devices->isPressed('S'))
	{
		auto a = ControlEvent(ControlEvent::MoveDown, dt);
		_controller->onEvent(&a);
	}
	if (_input_devices->isPressed('A'))
	{
		auto a = ControlEvent(ControlEvent::MoveLeft, dt);
		_controller->onEvent(&a);
	}
	if (_input_devices->isPressed('D'))
	{
		auto a = ControlEvent(ControlEvent::MoveRight, dt);
		_controller->onEvent(&a);
	}
	if (_input_devices->isPressed(MK_LBUTTON))
	//if (_input_devices->isPressed(VK_SPACE))
	{
		int mx = _input_devices->mouseX();
		int my = _input_devices->mouseY();
		ControlEvent event_(ControlEvent::FirePrimary, dt);
		//OutputDebugString(TEXT("\nmx,my"));
		//OutputDebugString(std::to_wstring(mx).c_str());
		//OutputDebugString(std::to_wstring(my).c_str());
		event_.setMouseCoords(mx, my);
		_controller->onEvent(&event_);
	}
}
