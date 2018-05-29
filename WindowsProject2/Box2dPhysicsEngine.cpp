#include "Box2dPhysicsEngine.h"
#include "GameObject.h"
#include "CollisionEvent.h"
#include "DebugLog.h"

#include "Projectile.h"
#include "Particle.h"

// Collsion masks
#define FRIENDLY_PROJECTILE_MASK 0x0002
#define ROBOT_MASK               0x0004
#define SLIME_MASK               0x0008
#define IMPASSABLE_TERRAIN       0x0010
#define WALL_MASK                0x0020
#define PARTICLE_MASK            0x0040 
#define COLLIDE_WITH_ALL         0xFFFF


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
	if (obj->getFineCollisionBoxes().isEmpty())
		return;

	b2Vec2 pos = obj->getWorldCenterCoords();
	pos.x = convertPxToMeters(pos.x);
	pos.y = convertPxToMeters(pos.y);
	// TODO: sort this out
	b2BodyDef groundBodyDef; // this doesn't need to stick around
	groundBodyDef.position.Set(pos.x, pos.y);

	b2Body* groundBody = _world.CreateBody(&groundBodyDef);
	_ptr_to_bdy[obj] = groundBody;
	groundBody->SetUserData(obj);
	
	// Get hit box from game obj
	RECTF_TYPE box = obj->getFineCollisionBoxes().at(0);
	float half_w = (box.right - box.left) *0.5f;
	float half_h = (box.bottom - box.top) *0.5f;
	half_w = convertPxToMeters(half_w);
	half_h = convertPxToMeters(half_h);

	// b2PolygonShape groundBox; // old way
	b2PolygonShape groundBox; 
	groundBox.SetAsBox(half_w, half_h);
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &groundBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	fixtureDef.filter.categoryBits = WALL_MASK;
	fixtureDef.filter.maskBits     = (UINT16)(~COLLIDE_WITH_ALL);

	groundBody->CreateFixture(&groundBox, 0.0f);
}

void Box2dPhysicsEngine::addDynamicGameObject(GameObject* obj)
{
	const static GString slime_str("Slime");
	const static GString robot_str("Robot");

	// Get position from game obj
	b2Vec2 pos = obj->getWorldCenterCoords();
	pos.x = convertPxToMeters(pos.x);
	pos.y = convertPxToMeters(pos.y);

	// Get hit box from game obj
	RECTF_TYPE box = obj->getFineCollisionBoxes().at(0);
	float half_w = (box.right - box.left) *0.5f;
	float half_h = (box.bottom - box.top) *0.5f;
	half_w = convertPxToMeters( half_w );
	half_h = convertPxToMeters( half_h );

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);

	b2Body* body = _world.CreateBody(&bodyDef);
	_ptr_to_bdy[obj] = body;

	body->SetUserData(obj);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(half_w, half_h);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density =  (obj->getObjectType() == robot_str) ? 1.0f : 0.5f;
	fixtureDef.friction = 0.3f;

	if (obj->getObjectType() == robot_str)
	{
		fixtureDef.filter.categoryBits = ROBOT_MASK;
		fixtureDef.filter.maskBits     = ~FRIENDLY_PROJECTILE_MASK;
	}
	else if (obj->getObjectType() == slime_str)
	{
		fixtureDef.filter.categoryBits = SLIME_MASK;
		fixtureDef.filter.maskBits = COLLIDE_WITH_ALL;
	}

	body->CreateFixture(&fixtureDef);
}

void Box2dPhysicsEngine::addProjectile(GameObject* obj)
{
	Projectile* proj = static_cast<Projectile*>(obj);
		// Get position from game obj
	b2Vec2 pos = proj->getWorldCenterCoords();
	pos.x = convertPxToMeters(pos.x);
	pos.y = convertPxToMeters(pos.y);

	// Get hit box from game obj
	//RECTF_TYPE box = obj->getBoundingRect(); // old and bad
	/*RECTF_TYPE box = proj->getFineCollisionBoxes().at(0);
	float half_w = (box.right - box.left) * 0.5f;
	float half_h = (box.bottom - box.top) * 0.5f;
	half_w = convertPxToMeters( half_w );
	half_h = convertPxToMeters( half_h );*/

	RECTF_TYPE box = obj->getFineCollisionBoxes().at(0);
	float half_w = (box.right - box.left) *0.5f;
	float half_h = (box.bottom - box.top) *0.5f;
	half_w = convertPxToMeters(half_w);
	half_h = convertPxToMeters(half_h);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	//bodyDef.bullet = true;
	bodyDef.bullet = false;
	bodyDef.position.Set(pos.x, pos.y);
	
	bodyDef.linearVelocity = proj->getMoveForce();

	b2Body* body = _world.CreateBody(&bodyDef);
	_ptr_to_bdy[obj] = body;

	body->SetUserData(proj);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(half_w, half_h);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density =  0.5f;
	fixtureDef.friction = 0.3f;
	fixtureDef.filter.categoryBits = FRIENDLY_PROJECTILE_MASK;
	fixtureDef.filter.maskBits     = ~FRIENDLY_PROJECTILE_MASK;

	body->CreateFixture(&fixtureDef);
}

void Box2dPhysicsEngine::addParticleObject(GameObject* obj)
{
	Particle* particle = static_cast<Particle*>(obj);
	// Get position from game obj
	b2Vec2 pos = particle->getWorldCenterCoords();
	pos.x = convertPxToMeters(pos.x);
	pos.y = convertPxToMeters(pos.y);

	// Get hit box from game obj
	RECTF_TYPE box = particle->getBoundingRect();
	float half_w = (box.right - box.left) *0.5f;
	float half_h = (box.bottom - box.top) *0.5f;
	half_w = convertPxToMeters(half_w);
	half_h = convertPxToMeters(half_h);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.linearVelocity = particle->getVelocity();
	bodyDef.angle = particle->getAngle();
	bodyDef.angularVelocity = particle->getAngularVelocity();

	b2Body* body = _world.CreateBody(&bodyDef);
	_ptr_to_bdy[particle] = body;

	body->SetUserData(particle);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(half_w, half_h);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 0.001f;
	fixtureDef.friction = 0.3f;

	fixtureDef.filter.categoryBits = PARTICLE_MASK;
	fixtureDef.filter.maskBits = ~(FRIENDLY_PROJECTILE_MASK | IMPASSABLE_TERRAIN);

	body->CreateFixture(&fixtureDef);
}

void Box2dPhysicsEngine::reset(GameObject* obj)
{
	if (!_ptr_to_bdy.contains(obj))
		return;
	if (_ptr_to_bdy[obj] == nullptr)
		return;

	b2Body *b = _ptr_to_bdy[obj];
	b2Vec2 pos = obj->getWorldCenterCoords();
	pos.x = convertPxToMeters(pos.x);
	pos.y = convertPxToMeters(pos.y);
	b->SetTransform(b2Vec2(pos.x, pos.y), b->GetAngle());
	b->SetActive(true);
	b->SetAwake(true);
	b->SetLinearVelocity(obj->getMoveForce());


	//body->SetTransform(b2Vec2(0, 0), body->GetAngle())
	//destroyBody(obj);
	//addProjectile(obj);
}

void Box2dPhysicsEngine::destroyBody(GameObject* obj)
{
	if (!_ptr_to_bdy.contains(obj))
		return;
	if (_ptr_to_bdy[obj] == nullptr)
		return;

	_ptr_to_bdy[obj]->SetActive(false);
	_world.DestroyBody(_ptr_to_bdy[obj]);
	_ptr_to_bdy[obj] = nullptr;
}

void Box2dPhysicsEngine::update(__int64 dt)
{
	float32 timeStep = static_cast<float32>((double)dt/1E6);

	// 8 and 3 are ideal.. more iterations is more accuracy less speed
	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	_world.Step(timeStep, velocityIterations, positionIterations);

	for (b2Contact* c = _world.GetContactList(); c; c = c->GetNext())
	{
		// OK, I think this thing will only generate 1 of these per collision instead of 1 for each object colliding..
		// I guess it makes more sense in hindsight
		if (!c->IsTouching())
			continue;

		GameObject* obj = static_cast<GameObject*>(c->GetFixtureA()->GetBody()->GetUserData());
		GameObject* collider = static_cast<GameObject*>(c->GetFixtureB()->GetBody()->GetUserData());
		//assert(c->IsTouching());
		
		obj->onEvent(&CollisionEvent(collider));
		collider->onEvent(&CollisionEvent(obj));
	}

	for (b2Body* b = _world.GetBodyList(); b; b = b->GetNext())
	{
		if (!b->IsActive())
			continue;

		GameObject* obj = static_cast<GameObject*>(b->GetUserData());
		assert(obj);
		if (!obj->isActive())
		{
			b->SetActive(false);
			continue;
		}

		float x = b->GetPosition().x;
		float y = b->GetPosition().y;

		x = convertMetersToPx(x);
		y = convertMetersToPx(y);

		obj->setWorldCenterCoords(x,y);
		b->SetLinearVelocity(obj->getMoveForce());
		//float32 angle = body->GetAngle();
	}


}

float Box2dPhysicsEngine::convertPxToMeters(float px)
{
	// 32px represents 1 meter, so 32px/1m = 1m/32px inverse scalar
	const static float scalar = 1.0f / 32.0f; 
	return px * scalar;
}

float Box2dPhysicsEngine::convertMetersToPx(float meters)
{
	// 32px represents 1 meter, so 32px/1m = 1m/32px inverse scalar
	const static float scalar = 32.0f / 1.0f;
	return meters * scalar;
}