#include "Explosion.h"
#include "ObjectFactory.h"
#include "Particle.h"
#include "DebugLog.h"
#include <stdlib.h>





float Explosion::randomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

Explosion::Explosion(VECTORF pos)
{
	emitter_location = pos;
}

Explosion::~Explosion()
{
}

void Explosion::destroy()
{
	_is_active = false;
	getObjectFactory()->releaseObject(this);
}

void Explosion::update(__int64 dt)
{
	for (int i = 0; i < 10; i++)
	{
		float vec_x = randomFloat(-10, 10);
		float vec_y = randomFloat(-10, 10);
		getObjectFactory()->createParticle(emitter_location, VECTORF(vec_x, vec_y), 0, 0, 5.0f, 1000000);
	}
	destroy();
}