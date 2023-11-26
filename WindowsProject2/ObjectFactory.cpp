#include "ObjectFactory.h"

#include "GameLoop.h"
#include "GraphicsEngine.h"
#include "PhysicsEngine.h"
#include "SpriteSheetManager.h"
#include "BirthEvent.h"

#include "Robot.h"
#include "Slime.h"
#include "SpinnyBox.h"
#include "BackGroundTileDisplayable.h"
#include "Projectile.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "Explosion.h"
#include "Camera.h"

#include "DebugLog.h"
#include <assert.h>



ObjectFactory& ObjectFactory::Instance()
{
	static ObjectFactory instance;
	return instance;
}

void ObjectFactory::initialize(GameLoop* gloop, GraphicsEngine* rengine, SpriteSheetManager* sprite_manager, AudioEngine* audio_engine, PhysicsEngine* physics_engine)
{
	_game_loop      = gloop;
	_render_engine  = rengine;
	_sprite_manager = sprite_manager;
	_audio_engine   = audio_engine;
	_physics_engine = physics_engine;
}

ObjectFactory::~ObjectFactory()
{
	for (int i = 0; i < _all_objects.count(); i++)
		delete _all_objects[i];
}

Projectile* ObjectFactory::createProjectile(float x, float y, float target_x, float target_y)
{// TODO: clean this up
	_render_engine->getCamera()->convertScreenCoordToWorldCoord(target_x, target_y, target_x, target_y);
	const static GString projectile_str("Projectile");

	if (!hasIdleEquivalent(projectile_str))
	{
		DebugLog::Instance().log("Create");
		// create
		Projectile* proj = new Projectile(x, y, target_x, target_y);
		proj->setAudioEngine(_audio_engine);
		proj->setObjectFactory(this);

		proj->setSprite(_sprite_manager->getSpriteFromID(38));
		for (int i = 0; i < _sprite_manager->getHitBoxesFromID(38).count(); i++)
			proj->addTinyCollisionBoxesForStage2Detect(_sprite_manager->getHitBoxesFromID(38).at(i));

		proj->setSolid(true);
		_game_loop->addUpdatableObject(proj);
		_render_engine->addDisplayableObject(proj, 1);
		BirthEvent event;
		proj->onEvent(&event);
		_physics_engine->addProjectile(proj);
		_all_objects.append(proj);
		return proj;
	}
	else
	{
		// recycle
		DebugLog::Instance().log("Reuse");
		Projectile* proj = static_cast<Projectile*>(_idle_objects[projectile_str].pop());
		proj->setWorldCoordinates(x,y);
		proj->setTarget(target_x, target_y);
		proj->setActive(true);
		BirthEvent event;
		proj->onEvent(&event);
		_physics_engine->reset(proj);

		return proj;
	}

}

bool ObjectFactory::hasIdleEquivalent(const GString& obj_type)
{
	return (_idle_objects.contains(obj_type) && !_idle_objects[obj_type].isEmpty());
}

void ObjectFactory::releaseObject(GameObject * object)
{
	object->setActive(false); 
	_idle_objects[object->getObjectType()].push(object);
}

GameObject* ObjectFactory::createRobot(float x, float y, int layer)
{
	// TODO: figure out how to organize keyboard
	Robot *thing = new Robot(x, y, 32, 32, this);
	thing->setSprite(_sprite_manager->getSpriteFromID(47));
	for (int boxes = 0; boxes < _sprite_manager->getHitBoxesFromID(47).count(); boxes++)
		thing->addTinyCollisionBoxesForStage2Detect(_sprite_manager->getHitBoxesFromID(47).at(boxes));

	_render_engine->addDisplayableObject(thing, layer);

	// Camera stuff might need to be moved outside
	_render_engine->getCamera()->centerScreenOnThisGuy(thing); // put this back
	_game_loop->addUpdatableObject(_render_engine->getCamera());
	_game_loop->addUpdatableObject(thing);
	_physics_engine->addDynamicGameObject(thing);
	_all_objects.append(thing);
	return thing;
}

GameObject* ObjectFactory::createSlime(float x, float y, int layer)
{
#if 1
	Slime *thing = new Slime(x, y, 32, 32);
	// TODO: this shouldn't be hard coded
	for (int boxes = 0; boxes < _sprite_manager->getHitBoxesFromID(37).count(); boxes++)
	{
		thing->addTinyCollisionBoxesForStage2Detect(_sprite_manager->getHitBoxesFromID(37).at(boxes));
	}
	thing->setSprite(_sprite_manager->getSpriteFromID(37));

	_render_engine->addDisplayableObject(thing, layer);
	_game_loop->addUpdatableObject(thing);
	_physics_engine->addDynamicGameObject(thing);
	thing->setObjectFactory(this);
	_all_objects.append(thing);
#endif
	return thing;
}

GameObject * ObjectFactory::createBackgroundTile(float x, float y, float w, float h, int id, int layer)
{
	DisplayableBitmap* thing = new BackGroundTileDisplayable(x,y,w,h);
	
	if (!_sprite_manager->getHitBoxesFromID(id).isEmpty())
	{
		thing->setSolid(true);

		for (int rect = 0; rect < _sprite_manager->getHitBoxesFromID(id).count(); rect++)
			thing->addTinyCollisionBoxesForStage2Detect(_sprite_manager->getHitBoxesFromID(id).at(rect));
		if (_sprite_manager->getHitBoxesFromID(id).count())
			_physics_engine->addStaticGameObject(thing);
	}

	Sprite sprite = _sprite_manager->getSpriteFromID(id);
	thing->setSprite(sprite);

	_render_engine->addDisplayableObject(thing, layer);
	_game_loop->addUpdatableObject(thing);
	_all_objects.append(thing);
	return thing;
}

GameObject* ObjectFactory::createSpinnyBox(float x, float y, int id, int layer)
{
	DisplayableAnimation* thing = new SpinnyBox();
	loadUpGameObjectWithPtrs(thing);

	if (!_sprite_manager->getHitBoxesFromID(id).isEmpty())
	{
		thing->setSolid(true);
		for (int rect = 0; rect < _sprite_manager->getHitBoxesFromID(id).count(); rect++)
			thing->addTinyCollisionBoxesForStage2Detect(_sprite_manager->getHitBoxesFromID(id).at(rect));
	}

	Sprite sprite = _sprite_manager->getSpriteFromID(id);
	thing->setSprite(sprite);

	thing->setWorldCoordinates(x, y);
	_render_engine->addDisplayableObject(thing, layer);
	_game_loop->addUpdatableObject(thing);

	// let it know it exists now
	auto a = BirthEvent();
	thing->onEvent(&a);
	_all_objects.append(thing);
	return thing;
	
}

GameObject * ObjectFactory::createCamera()
{
	static Camera camera;
	return &camera;
}

GameObject* ObjectFactory::createParticle(VECTORF location, VECTORF velocity, float angle, float angle_velocity, float sizes, float life_time)
{
	const static GString particle_str("Particle");
	Particle* result = nullptr;
	if (hasIdleEquivalent(particle_str))
	{
		// recycle
		DebugLog::Instance().log("Recycling particle");
		result  = static_cast<Particle*>(_idle_objects[particle_str].pop());
		result->setActive(true);
	}
	else
	{
		// create
		DebugLog::Instance().log("Creating particle");
		result = new Particle();
		result->setObjectFactory(this);
		_render_engine->addDisplayableObject(result);
		_game_loop->addUpdatableObject(result);
		_all_objects.append(result);
	}

	result->setPosition(location);
	result->setVelocity(velocity);
	result->setAngle(angle);
	result->setAngularVelocity(angle_velocity);
	result->setSize(sizes);
	result->setLifeTime(life_time);

	_physics_engine->addParticleObject(result); // gonna have to figure out where to put this one...
	
	return result;
}

GameObject* ObjectFactory::createParticleEmitter(float x, float y)
{
	Explosion *result = new Explosion(VECTORF(x, y));
	result->setObjectFactory(this);
	_game_loop->addUpdatableObject(result);
	_all_objects.append(result);
	return result;
}

void ObjectFactory::loadUpGameObjectWithPtrs(GameObject* obj)
{
	obj->setSpriteManager(_sprite_manager);
	obj->setAudioEngine(_audio_engine);
	obj->setObjectFactory(this);
	obj->setCamera(_render_engine->getCamera());
}














