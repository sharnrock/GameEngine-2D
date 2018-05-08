#include "GameLoop.h"
#include "Updatable.h"

#include "GameObject.h"
#include "CollisionEvent.h"

GameLoop::GameLoop() :
	_updatables(1)
{
}


GameLoop::~GameLoop()
{
}

void GameLoop::update(__int64 delta_t_us)
{
	for (int i = 0; i < _updatables.count(); i++)
	{
		_updatables[i]->update(delta_t_us);
	}
	updateCollisions(delta_t_us);
}

void GameLoop::updateCollisions(__int64 delta_t_us)
{
#if 0
	for (int i = 0; i < _updatables.count(); i++)
	{
		GameObject* one = dynamic_cast<GameObject*>(_updatables[i]);
		for (int y = 0; y < _updatables.count(); y++)
		{
			GameObject* two = dynamic_cast<GameObject*>(_updatables[y]);
			if (one->hasCoarseCollisionWith(*two))
			{
				CollisionEvent event(two);
				one->onEvent(&event);
			}
		}
	}
#else
	// could potentially use multiple KD-trees to detect whatever is colliding
	// quad trees too

	// This is still n^2 time
	// Try to see if grid or tree struc can do this faster
	// Grid probably won't work because I've thrown all the tiles into a list in an arbitrary order
	for (int i = 0; i < _collideables.count(); i++)
	{
		GameObject* one = _collideables[i];
		for (int y = 0; y < _collideables.count(); y++)
		{
			if (i == y)
				continue;

			GameObject* two = _collideables[y];
			if (one->hasCoarseCollisionWith(*two) && one->hasFineCollisionWith(*two))
			{
				CollisionEvent event(two);
				one->onEvent(&event);
			}
		}
	}
#endif
}

void GameLoop::addUpdatableObject(GameObject* updatable_object)
{
	_updatables.append(updatable_object);
	if (updatable_object->isSolid())
		_collideables.append(updatable_object);
}


