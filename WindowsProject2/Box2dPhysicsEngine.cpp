#include "Box2dPhysicsEngine.h"
#include "GameObject.h"
#include "CollisionEvent.h"
#include "DebugLog.h"


Box2dPhysicsEngine::Box2dPhysicsEngine() :
	_gravity(0.0f, 0.0f),
	_world(_gravity)
{
}


Box2dPhysicsEngine::~Box2dPhysicsEngine()
{
}

void Box2dPhysicsEngine::initialize()
{
	DebugLog::Instance().log("Initializing Physics Engine");
	// static bodies don't move
	// b2_kinematicBody they move, but don't collide with static, I think they work best for platforms.  They have ininite mass

	// dnynamic bodies move

	// TOOD: only delete world, everything else like bodies and fixtures gets deleted with it
	// takes ownership of everything

	// TIPS
	// you'll need to enable fixed rotation on most stuff
	// bodyDef.bullet = true; if it's a bullet..
	// bodyDef.active = true; deactivate stuff to make it not collide.
	// bodyDef.userData = &myActor; throw some extra information in here void* throw some stuf in here
	// collisions between fixtures can be filtered.. so bullets can collide with walls and not water, but players will collide with water and bullets and walls.
	// sensors will let you know when stuff collides, but will not react or simulate physics
	// might be able to use sensors for when enemies detect stuff
	// collsions are called contacts and there is a contact listener that will make bullets/players hit stuff
	// use ray casts for detecting hitscan

	

}


void Box2dPhysicsEngine::addStaticGameObject(GameObject* obj)
{
	float x = convertPxToMeters(obj->X());
	float y = convertPxToMeters( obj->Y()); 
	// TODO: sort this out
	b2BodyDef groundBodyDef; // this doesn't need to stick around
	groundBodyDef.position.Set(x, y);

	b2Body* groundBody = _world.CreateBody(&groundBodyDef);
	groundBody->SetUserData(obj);
	
	// Get hit box from game obj
	RECTF_TYPE box = obj->getBoundingRect();
	float half_w = (box.right - box.left) * 0.5f;
	float half_h = (box.bottom - box.top) * 0.5f;
	half_w = convertPxToMeters(half_w);
	half_h = convertPxToMeters(half_h);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(half_w, half_h);
	
	groundBody->CreateFixture(&groundBox, 0.0f);
}

void Box2dPhysicsEngine::addDynamicGameObject(GameObject* obj)
{
	// Get position from game obj
	float x = convertPxToMeters( obj->X() );
	float y = convertPxToMeters( obj->Y() ); // temp for testing

	// Get hit box from game obj
	//RECTF_TYPE box = obj->getBoundingRect(); // old and bad
	RECTF_TYPE box = obj->getFineCollisionBoxes().at(0);
	float half_w = (box.right - box.left) * 0.5f;
	float half_h = (box.bottom - box.top) * 0.5f;
	half_w = convertPxToMeters( half_w );
	half_h = convertPxToMeters( half_h );

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);

	if (obj->getObjectType() == GString("Robot"))
		bodyDef.linearVelocity.Set(1.4f, 0);
	else
		bodyDef.linearVelocity.Set(-1.4f, 0);

	b2Body* body = _world.CreateBody(&bodyDef);

	body->SetUserData(obj);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(half_w, half_h);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density =  (obj->getObjectType() == GString("Robot")) ? 1.0f : 0.5f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);
}

void Box2dPhysicsEngine::update(__int64 dt)
{
	float32 timeStep = static_cast<float32>((double)dt/1E6);

	// 8 and 3 are ideal.. more iterations is more accuracy less speed
	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	_world.Step(timeStep, velocityIterations, positionIterations);

	for (b2Body* b = _world.GetBodyList(); b; b = b->GetNext())
	{
		//if (b->GetType() == b2_staticBody)
		//	continue;
		if (!b->IsActive())
			continue;
		// TODO: bring this back once things get moving!
		//if (!b->IsAwake())
		//	continue;

		GameObject* obj = static_cast<GameObject*>(b->GetUserData());
		assert(obj);
		float x = b->GetPosition().x;
		float y = b->GetPosition().y;
		x = convertMetersToPx(x);
		y = convertMetersToPx(y);
		obj->setWorldCoordinates(x,y);
		//b->ApplyForceToCenter(obj->getMoveForce(), true);
		b->SetLinearVelocity(obj->getMoveForce());
		//float32 angle = body->GetAngle();
	}

	for (b2Contact* c = _world.GetContactList(); c; c = c->GetNext())
	{
		GameObject* obj      = static_cast<GameObject*>( c->GetFixtureA()->GetBody()->GetUserData() );
		GameObject* collider = static_cast<GameObject*>( c->GetFixtureB()->GetBody()->GetUserData() );
		CollisionEvent cevent(collider);
		obj->onEvent(&cevent);
	}
}

float Box2dPhysicsEngine::convertPxToMeters(float px)
{
	// 32px represents 1 meter, so 32px/1m = 1m/32px inverse scalar
	const float scalar = 1.0f / 32.0f; 
	return px * scalar;
}

float Box2dPhysicsEngine::convertMetersToPx(float meters)
{
	// 32px represents 1 meter, so 32px/1m = 1m/32px inverse scalar
	const float scalar = 32.0f / 1.0f;
	return meters * scalar;
}