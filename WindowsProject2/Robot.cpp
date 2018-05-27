#include "Robot.h"
#include "Possessor.h"
#include "CollisionEvent.h"
#include "ObjectFactory.h"
#include "ControlEvent.h"
#include "Slime.h"
#include "DebugLog.h"


Robot::Robot(float x, float y, float w, float h, ObjectFactory* obj_factory) :
	DisplayableBitmap(x, y, w, h),
	_possessor(nullptr),
	speed_px_s(100),
	_obj_factory(obj_factory),
	_weapon_timer(0),
	_weapon_cooldown(100000),
	_acceleration(4.0),
	_weapon(nullptr),
	_health(10)
{
	_obj_type = "Robot";
}

Robot::~Robot()
{
}

void Robot::moveLeft(__int64 dt)
{
	_move_force.x = -1 * _acceleration;
	updateBoundingRect();
}

void Robot::moveUp(__int64 dt)
{
	_move_force.y = -1 * _acceleration;
	updateBoundingRect();
}

void Robot::moveRight(__int64 dt)
{
	_move_force.x = _acceleration;
	updateBoundingRect();
}

void Robot::moveDown(__int64 dt)
{
	_move_force.y = _acceleration;
	updateBoundingRect();
}

void Robot::firePrimary(int mouse_x, int mouse_y)
{
	if (_weapon_timer <= 0)
	{
		_obj_factory->createProjectile(_x + 30.0f, _y+9.0f, (float)mouse_x, (float)mouse_y); // fires to the right
		_weapon_timer = _weapon_cooldown;
	}
}

void Robot::setPossesor(Possessor* possessor)
{
	_possessor = possessor;
	_possessor->setControllable(this);
}

void Robot::update(__int64 dt)
{
	//if (_health < 0)
	//{
	//	DebugLog::Instance().error("You are dead");
	//	exit(1);
	//}

	// update the weapon cooldown
	_weapon_timer = (_weapon_timer > 0) ? _weapon_timer - dt : 0;
	
	//_move_force.x -= dt * _acceleration / 1E6F;
	_move_force.SetZero();
	_possessor->update(dt);
}

const static float x_center_offset = -4.0f;
const static float y_center_offset = -8.0f;
void Robot::setWorldCenterCoords(float x, float y)
{
	x -= x_center_offset;
	y -= y_center_offset;
	GameObject::setWorldCenterCoords(x,y);
}

VECTORF Robot::getWorldCenterCoords() const
{// less y and x
	VECTORF result = GameObject::getWorldCenterCoords();
	result.x += x_center_offset;
	result.y += y_center_offset;
	return result;
}

void Robot::equipWeapon(Weapon* weapon)
{
	_weapon = weapon;
	// TODO: make all that on create stuff so it'll make noise when equpped or soemthing...
}

void Robot::onCollisionEvent(CollisionEvent* e)
{
	const static GString slime_str("Slime");
	if (e->getCollider()->getObjectType() == slime_str)
	{
		_health -= static_cast<Slime*>(e->getCollider())->getDamage();
	}
}

void Robot::onControlEvent(ControlEvent* e)
{
	switch (e->getControl())
	{
	case ControlEvent::MoveDown:
		moveDown(e->getDt());
		break;
	case ControlEvent::MoveLeft:
		moveLeft(e->getDt());
		break;
	case ControlEvent::MoveRight:
		moveRight(e->getDt());
		break;
	case ControlEvent::MoveUp:
		moveUp(e->getDt());
		break;
	case ControlEvent::FirePrimary:
		firePrimary(e->getMouseX(), e->getMouseY());
		break;
	}
}