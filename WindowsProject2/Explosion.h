#pragma once
#include "GameObject.h"
class Explosion :
	public GameObject
{
public:
	Explosion(VECTORF pos);
	~Explosion();

	void setEmitterLocation(VECTORF pos) { this->emitter_location = pos; }
	void update(__int64);

private:
	float randomFloat(float a, float b);
	void destroy();

	VECTORF emitter_location;
	// random stuff
};

