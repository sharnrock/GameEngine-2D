#include "Particle.h"
#include "ObjectFactory.h"



Particle::Particle()
{
}


Particle::~Particle()
{
}

void Particle::setPosition(VECTORF position) 
{ 
	this->position = position; 
	setWorldCoordinates(position.x, position.y);
}

void Particle::setVelocity(VECTORF velocity) 
{ 
	this->velocity = velocity; 
	this->_move_force = velocity;
}

void Particle::setSize(float size)
{ 
	this->size = size; 
	_w = size;
	_h = size;
	updateBoundingRect();
}

void Particle::update(__int64 dt) 
{ 
	if (life_time < 0)
	{
		_is_active = false;
		getObjectFactory()->releaseObject(this);
		return;
	}
	life_time -= dt;
}