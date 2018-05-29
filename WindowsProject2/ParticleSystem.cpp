#include "ParticleSystem.h"
#include "ObjectFactory.h"
#include "Particle.h"
#include "DebugLog.h"
#include <stdlib.h>

float ParticleSystem::randomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

ParticleSystem::ParticleSystem() :
	particle_release_time_span(1000000),
	particle_release_timer(particle_release_time_span)
{
}

ParticleSystem::ParticleSystem(VECTORF pos) :
	ParticleSystem()
{
	emitter_location = pos;
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::destroy()
{
	_is_active = false;
}

void ParticleSystem::update(__int64 dt)
{
	particle_release_timer -= dt;

	if (particle_release_timer < 0)
	{
		particle_release_timer = particle_release_time_span;
		for (int i = 0; i < 10; i++)
		{
			float vec_x = randomFloat(-10, 10);
			float vec_y = randomFloat(-10, 10);
			getObjectFactory()->createParticle(emitter_location, VECTORF(vec_x, vec_y), 0, 0, 5.0f, 1000000);
		}
		destroy();
	}

	
}


