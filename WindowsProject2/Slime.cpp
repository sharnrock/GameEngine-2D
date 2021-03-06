#include "Slime.h"
#include "Possessor.h"
#include "ControlEvent.h"
#include "CollisionEvent.h"
#include "Projectile.h"
#include "DebugLog.h"
#include "ObjectFactory.h"


Slime::Slime(float x, float y, float w, float h) :
	DisplayableBitmap(x,y,w,h),
	_possessor(nullptr),
	speed_px_s(30),
	_acceleration(2.0),
	_health(30),
	_damage(10)
{
	_obj_type = "Slime";
}

Slime::~Slime()
{
}

void Slime::moveLeft(__int64 dt)
{
	_move_force.x = -1 * _acceleration;
	updateBoundingRect();
}

void Slime::moveUp(__int64 dt)
{
	_move_force.y = -1 * _acceleration;
	updateBoundingRect();
}

void Slime::moveRight(__int64 dt)
{
	_move_force.x = 1 * _acceleration;
	updateBoundingRect();
}

void Slime::moveDown(__int64 dt)
{
	_move_force.y = 1 * _acceleration;
	updateBoundingRect();
}

void Slime::setPossesor(Possessor* possessor)
{
	_possessor = possessor;
	_possessor->setControllable(this);
}

void Slime::update(__int64 dt)
{
	if (_health < 0)
	{
		DebugLog::Instance().log("Slime has died!");
		_is_active = false;
		this->getObjectFactory()->createParticleEmitter(X(), Y());
		//_move_force.SetZero();

		getObjectFactory()->releaseObject(this);
		return;
	}
	_move_force.SetZero();
	_possessor->update(dt);
}

void Slime::onCollisionEvent(CollisionEvent* e)
{
	if (e->getCollider()->getObjectType() == "Projectile")
	{
		_health -= static_cast<Projectile*>(e->getCollider())->getDamage();
	}
}

void Slime::onControlEvent(ControlEvent* e)
{
	switch (e->getControl())
	{
	case ControlEvent::MoveLeft:
		moveLeft(e->getDt());
		break;
	case ControlEvent::MoveRight:
		moveRight(e->getDt());
		break;
	case ControlEvent::MoveUp:
		moveUp(e->getDt());
		break;
	case ControlEvent::MoveDown:
		moveDown(e->getDt());
		break;
	}
	// Doesn't do anything else yet..
}
