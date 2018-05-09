#include "ObjectFactory.h"
#include "Projectile.h"
#include "GameLoop.h"
#include "RenderEngine.h"

ObjectFactory::ObjectFactory(GameLoop* gloop, RenderEngine* rengine) :
	_game_loop(gloop),
	_render_engine(rengine)
{
}


ObjectFactory::~ObjectFactory()
{
}

void ObjectFactory::setCollisionForProjectile(const RECTF_TYPE& collision)
{
	_collision = collision;
}

void ObjectFactory::setSpriteForProjectile(const Sprite& sprite)
{
	_projectile_sprite = sprite;
}

Projectile* ObjectFactory::createProjectile(float x, float y, float target_x, float target_y)
{
	// Eventually this will just be grabbed from the object pool 
	Projectile* proj = new Projectile(x, y, target_x, target_y);

	proj->setSprite(_projectile_sprite);
	proj->addTinyCollisionBoxesForStage2Detect(_collision);
	proj->setSolid(true);

	_game_loop->addUpdatableObject(proj);
	_render_engine->addDisplayableObject(proj,0); // TODO: 9 for the time being 

	return proj;
}