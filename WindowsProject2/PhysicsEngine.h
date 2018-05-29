#pragma once

class GameObject;

class PhysicsEngine
{
public:
	PhysicsEngine();
	virtual ~PhysicsEngine();

	virtual void addDynamicGameObject(GameObject* obj) = 0;
	virtual void addStaticGameObject(GameObject* obj) = 0;
	virtual void addKinematicGameObject(GameObject* obj) = 0;
	virtual void addProjectile(GameObject* obj) = 0;
	virtual void addParticleObject(GameObject* obj) = 0;
	virtual void reset(GameObject* obj) = 0;
	virtual void destroyBody(GameObject* obj) = 0;

	virtual void update(__int64 dt) = 0;
	virtual void initialize() = 0;
};

