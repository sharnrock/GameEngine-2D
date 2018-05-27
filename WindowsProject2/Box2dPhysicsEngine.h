#pragma once
#include "PhysicsEngine.h"
#include "Box2D\Box2D.h"

class GameObject;

// adapts the box2d physics engine to our program
class Box2dPhysicsEngine :
	public PhysicsEngine
{
public:
	Box2dPhysicsEngine();
	~Box2dPhysicsEngine();

	void initialize();
	void addStaticGameObject(GameObject* obj);
	void addDynamicGameObject(GameObject* obj);
	void addKinematicGameObject(GameObject* obj) {} // TOOD: implement later

	void update(__int64 dt);

private:
	float convertPxToMeters(float px); 
	float convertMetersToPx(float meters);


	b2Vec2  _gravity; // this might be able to be removed
	b2World _world; 
};


