#pragma once
#include "GameObject.h"

// Probably for stuff that works in the box2d engine
// Stuff that needs physics simulation, but not collisions or tator tots - #Liv
class PhysicsGameObject :
	public GameObject
{
public:
	PhysicsGameObject();
	~PhysicsGameObject();
};

