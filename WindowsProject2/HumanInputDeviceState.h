#pragma once

#include "GMap.h"
#include <Windows.h>

// holds the state of the keyboard and mouse
// If threading becomes a thing this can become threadsafe by using a mutex
class HumanInputDeviceState
{
public:
	HumanInputDeviceState();
	~HumanInputDeviceState();

	// returns state
	int& operator[](int key_code)
	{
		return _key_states[key_code];
	}

	// returns state
	const int& operator[](int key_code) const
	{
		return _key_states.at(key_code);
	}

	bool isPressed(int key_code) const
	{
		if (_key_states.contains(key_code))
			return ((*this)[key_code] == WM_KEYDOWN || (*this)[key_code] == WM_SYSKEYDOWN);
		return false;
	}

	void setMouseCoords(int xPos, int yPos)
	{
		_mouse_x = xPos;
		_mouse_y = yPos;
	}
	
	int mouseX() const { return _mouse_x; }
	int mouseY() const { return _mouse_y; }

private:
	HumanInputDeviceState(const HumanInputDeviceState&) = delete;
	HumanInputDeviceState(const HumanInputDeviceState&&) = delete;
	HumanInputDeviceState& operator=(const HumanInputDeviceState&) = delete;
	HumanInputDeviceState& operator=(const HumanInputDeviceState&&) = delete;

	int _mouse_x, _mouse_y;

	// holds windows vkey as key and WM_KEYDOWN or WM_KEYUP as value
	GMap<int, int> _key_states;
};

