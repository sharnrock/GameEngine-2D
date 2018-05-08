#pragma once

#include "Possessor.h"

class HumanInputDeviceState;

class HumanPossessor :
	public Possessor
{
public:
	HumanPossessor(HumanInputDeviceState* input_devices);
	~HumanPossessor();

	virtual void update(__int64 dt);
private:
	HumanInputDeviceState* _input_devices;
};

