#include "ObjectFactory.h"
#include "GameLoop.h"
#include "RenderEngine.h"

#include "Robot.h"
#include "Slime.h"
#include "BackGroundTileDisplayable.h"
#include "Projectile.h"

#include "Camera.h"
#include "RenderEngine.h"
#include "SpriteSheetManager.h"


#include <assert.h>

ObjectFactory::ObjectFactory(GameLoop* gloop, RenderEngine* rengine, SpriteSheetManager* sprite_manager) :
	_game_loop(gloop),
	_render_engine(rengine),
	_sprite_manager(sprite_manager),
	_objects(256)
{
}

ObjectFactory::~ObjectFactory()
{
}

Projectile* ObjectFactory::createProjectile(float x, float y, float target_x, float target_y)
{
	// Eventually this will just be grabbed from the object pool 
	Projectile* proj = new Projectile(x, y, target_x, target_y);

	proj->setSprite(_sprite_manager->getSpriteFromID(42));
	for (int i = 0; i < _sprite_manager->getHitBoxesFromID(42).count(); i++)
		proj->addTinyCollisionBoxesForStage2Detect(_sprite_manager->getHitBoxesFromID(42).at(i));

	proj->setSolid(true);

	_game_loop->addUpdatableObject(proj);
	_render_engine->addDisplayableObject(proj,1); 

	return proj;
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
	thing->setSprite(_sprite_manager->getSpriteFromID(37));
	for (int boxes = 0; boxes < _sprite_manager->getHitBoxesFromID(37).count(); boxes++)
		thing->addTinyCollisionBoxesForStage2Detect(_sprite_manager->getHitBoxesFromID(37).at(boxes));

	_render_engine->addDisplayableObject(thing, layer);

	// Camera stuff might need to be moved outside
	_render_engine->getCamera()->centerScreenOnThisGuy(thing);
	_game_loop->addUpdatableObject(_render_engine->getCamera());
	_game_loop->addUpdatableObject(thing);

	return thing;
}

GameObject* ObjectFactory::createSlime(float x, float y, int layer)
{
#if 1
	Slime *thing = new Slime(x, y, 32, 32);
	// TODO: this shouldn't be hard coded
	for (int boxes = 0; boxes < _sprite_manager->getHitBoxesFromID(41).count(); boxes++)
	{
		thing->addTinyCollisionBoxesForStage2Detect(_sprite_manager->getHitBoxesFromID(41).at(boxes));
	}
	thing->setSprite(_sprite_manager->getSpriteFromID(41));

	_render_engine->addDisplayableObject(thing, layer);
	_game_loop->addUpdatableObject(thing);
#endif
	return thing;
}

GameObject * ObjectFactory::createBackgroundTile(float x, float y, int id, int layer)
{
	DisplayableBitmap* thing = new BackGroundTileDisplayable();

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

	return thing;
}
















