#pragma once

#include "Displayable.h"


class Particle :
	public Displayable
{
public:
	Particle();
	~Particle();

	DrawType getDrawType() const { return Rectangle; }

	// Set up initial parameters
	void setPosition(VECTORF position);
	void setVelocity(VECTORF velocity);
	void setAngle(float angle) { this->angle = angle; }
	void setAngularVelocity(float angular_velocity) { this->angular_velocity = angular_velocity; }
	void setSize(float size);
	void setLifeTime(float life_time) { this->life_time = life_time; }

	// Get initial parameters
	VECTORF getPosition() const { return this->position; }
	VECTORF getVelocity() const { return this->velocity; }
	float getAngle() const { return this->angle; }
	float getAngularVelocity() const { return this->angular_velocity; }
	float getSize() const { return this->size; }
	float getLifeTime() const { return this->life_time; }

	void update(__int64 dt);

private:
	// Initial values
	VECTORF position;
	VECTORF velocity;
	float angle;
	float angular_velocity;
	float size;
	float life_time;
};
