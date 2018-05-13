#pragma once

#include "Possessor.h"

class HumanInputDeviceState;

class HumanPossessor :
	public Possessor
{
public:
	HumanPossessor(HumanInputDeviceState* input_devices);
	~HumanPossessor();

	// This is where the actual key mappings to actions happen
	virtual void update(__int64 dt);
private:
	HumanInputDeviceState* _input_devices;
};

