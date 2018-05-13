#pragma once

#include "Event.h"
class GameObject;

class CollisionEvent :
	public Event
{
public:
	// This is the guy that collided with the object
	CollisionEvent(GameObject* collider);
	~CollisionEvent();

	Event::Type getType() const { return Event::Collision; }
	GameObject* getCollider() { return _collider; }

private:
	GameObject * _collider;
};

