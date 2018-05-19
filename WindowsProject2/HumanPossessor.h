#pragma once

#include "Possessor.h"

class HumanInputDeviceState;

class HumanPossessor :
	public Possessor
{
public:
	HumanPossessor(const HumanInputDeviceState* input_devices);
	~HumanPossessor();

	// This is where the actual key mappings to actions happen
	virtual void update(__int64 dt);
private:
	const HumanInputDeviceState* _input_devices;
};

