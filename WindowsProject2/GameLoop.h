#pragma once

#include "DynamicList.h"
class Updatable;
class GameObject;

class GameLoop
{
public:
	GameLoop();
	~GameLoop();

	void update(__int64 delta_t_us);

	// This creates N^2 time... we'll fix this soon
	void updateCollisions(__int64 delta_t_us);
	void addUpdatableObject(GameObject* updatable_object);

private:
	DynamicList<Updatable*> _updatables;
	DynamicList<GameObject*> _collideables;
};

