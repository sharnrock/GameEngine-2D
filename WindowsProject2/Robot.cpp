#include "Robot.h"
#include "Possessor.h"
#include "CollisionEvent.h"
#include "ObjectFactory.h"
#include "ControlEvent.h"

Robot::Robot(float x, float y, float w, float h, ObjectFactory* obj_factory) :
	DisplayableBitmap(x, y, w, h),
	_possessor(nullptr),
	speed_px_s(100),
	_obj_factory(obj_factory),
	_weapon_timer(0),
	_weapon_cooldown(100000),
	_acceleration(4.0),
	_weapon(nullptr)
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
		_obj_factory->createProjectile(_x + 60.0f, _y, (float)mouse_x, (float)mouse_y); // fires to the right
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

	updateBoundingRect();

#if 0
	static GString projectile_type("Projectile");

	if (e->getCollider()->getObjectType() == projectile_type)
		return;

	for (int y = 0; y < e->getCollider()->getFineCollisionBoxes().count(); y++)
	{
		RECTF_TYPE collider_box = e->getCollider()->getFineCollisionBoxes().at(y);
		collider_box.left += e->getCollider()->X();
		collider_box.right += e->getCollider()->X();
		collider_box.top += e->getCollider()->Y();
		collider_box.bottom += e->getCollider()->Y();

		for (int i = 0; i < _fine_collision_boxes.count(); i++)
		{
			RECTF_TYPE _bounding_rect = _fine_collision_boxes[i];

			_bounding_rect.left += _x;
			_bounding_rect.right += _x;
			_bounding_rect.top += _y;
			_bounding_rect.bottom += _y;

			if (!hasCollision(collider_box, _bounding_rect))
				continue;

			// create a vector to keep these things from overlapping
			float x_vect = _bounding_rect.right - collider_box.right;  // if neg this is left side collide
			//if (_bounding_rect.right > collider_box.right) // use this as replacement
			if (x_vect < 0)
			{
				// coming from left
				x_vect = collider_box.left - _bounding_rect.right;
				// push left with neg vect
			}
			else
			{
				// coming from right
				x_vect = collider_box.right - _bounding_rect.left;
				// push it right with pos vect
			}

			float y_vect;
			if (_bounding_rect.top < collider_box.top)
			{
				// coming from top
				y_vect = collider_box.top - _bounding_rect.bottom;
				// we want to push up with neg vect
			}
			else
			{
				// coming from bottom
				y_vect = collider_box.bottom - _bounding_rect.top;
				// we want to push down with pos vect
			}

			if (fabs(x_vect) < fabs(y_vect))
			{
				_x += x_vect;
			}
			else if (fabs(x_vect) > fabs(y_vect))
			{
				_y += y_vect;
			}
			else // they equal
			{
				_x += x_vect;
				_y += y_vect;
			}
		}
	}
#endif

	
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