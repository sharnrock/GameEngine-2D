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

	bool isPressed(int key_code)
	{
		return ((*this)[key_code] == WM_KEYDOWN || (*this)[key_code] == WM_SYSKEYDOWN);
	}

	void setMouseCoords(int xPos, int yPos)
	{
		_mouse_x = xPos;
		_mouse_y = yPos;
		//OutputDebugString(TEXT("x"));
		//OutputDebugString(TEXT("y"));
	}
	
	int mouseX() const { return _mouse_x; }
	int mouseY() const { return _mouse_y; }

private:
	int _mouse_x, _mouse_y;

	// holds windows vkey as key and WM_KEYDOWN or WM_KEYUP as value
	GMap<int,int> _key_states;
};

