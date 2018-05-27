#include "HumanInputDeviceState.h"



HumanInputDeviceState::HumanInputDeviceState()
{
	_key_states['W'] = WM_KEYUP;
	_key_states['A'] = WM_KEYUP;
	_key_states['S'] = WM_KEYUP;
	_key_states['D'] = WM_KEYUP;
	_key_states[VK_SPACE] = WM_KEYUP;
	_key_states[MK_LBUTTON] = WM_KEYUP;
}


HumanInputDeviceState::~HumanInputDeviceState()
{
}


