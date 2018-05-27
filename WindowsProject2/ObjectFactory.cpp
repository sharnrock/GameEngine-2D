#include "ObjectFactory.h"
#include "GameLoop.h"
#include "GraphicsEngine.h"
#include "PhysicsEngine.h"

#include "Robot.h"
#include "Slime.h"
#include "SpinnyBox.h"
#include "BackGroundTileDisplayable.h"
#include "Projectile.h"

#include "Camera.h"

#include "SpriteSheetManager.h"
#include "BirthEvent.h"

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
}

Projectile* ObjectFactory::createProjectile(float x, float y, float target_x, float target_y)
{
	_render_engine->getCamera()->convertScreenCoordToWorldCoord(target_x, target_y, target_x, target_y);
	if (_idle_projs.isEmpty())
	{
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

		_active_projs.append(proj);

		_physics_engine->addProjectile(proj);

		return proj;
	}
	else
	{
		// recycle
		Projectile* proj = _idle_projs.popBack();
		_active_projs.append(proj);
		proj->setWorldCoordinates(x,y);
		proj->setTarget(target_x, target_y);

		proj->setActive(true);
		BirthEvent event;
		proj->onEvent(&event);

		_physics_engine->reset(proj);

		return proj;
	}
}

void ObjectFactory::releaseProjectile(Projectile* obj)
{
	_physics_engine->destroyBody(obj);
	obj->setActive(false);
	_active_projs.remove(obj);
	_idle_projs.append(obj);
}

void ObjectFactory::releaseObject(GameObject * object)
{
	_objects.remove(object);
	delete object;
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

	// was here.
	_render_engine->addDisplayableObject(thing, layer);
	_game_loop->addUpdatableObject(thing);

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
	thing->onEvent(&BirthEvent());

	return thing;
	
}

GameObject * ObjectFactory::createCamera()
{
	static Camera camera;
	return &camera;
}

void ObjectFactory::loadUpGameObjectWithPtrs(GameObject* obj)
{
	obj->setSpriteManager(_sprite_manager);
	obj->setAudioEngine(_audio_engine);
	obj->setObjectFactory(this);
	obj->setCamera(_render_engine->getCamera());
}














