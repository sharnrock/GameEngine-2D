#pragma once
#include "GameObject.h"
class ParticleSystem :
	public GameObject
{
public:
	ParticleSystem();
	ParticleSystem(VECTORF pos); 
	~ParticleSystem();

	void setEmitterLocation(VECTORF pos) { this->emitter_location = pos; }
	void update(__int64);

private:
	float randomFloat(float a, float b);
	void destroy();

	VECTORF emitter_location;
	__int64 particle_release_time_span;
	__int64 particle_release_timer;
	// random stuff
};




