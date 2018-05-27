#pragma once

#include "GameObject.h"

class Weapon : 
	public GameObject
{
public:
	Weapon();
	virtual ~Weapon();

	// Should be called from class that equips it
	void update(__int64 dt);

protected: 
	__int64 _weapon_timer;
	__int64 _weapon_cooldown;
};

