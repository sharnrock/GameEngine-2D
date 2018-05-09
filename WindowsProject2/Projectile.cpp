#include "Projectile.h"
#include "CollisionEvent.h"



Projectile::Projectile(float x, float y, float target_x, float target_y) :
	DisplayableBitmap(x, y, 0, 0),
	_target_x(target_x),
	_target_y(target_y),
	_speed(400)
{
	updateAngle();
	_w = 32;
	_h = 32;
	updateBoundingRect();
}

void Projectile::updateAngle()
{
	// calc the angle from target coords
}


Projectile::~Projectile()
{
}



void Projectile::setSpeed(float speed)
{
	_speed = speed;
}

void Projectile::setTarget(float x, float y)
{
	_target_x = x;
	_target_y = y;
}

void Projectile::setAngle(float degrees)
{
	_angle_dgr = degrees;
}

void Projectile::update(__int64 dt)
{
	// TODO: put angle into this calc!
	_x += (float)(dt * _speed / 1E6f);
	updateBoundingRect();
}

void Projectile::onCollisionEvent(CollisionEvent* e)
{
	GameObject* obj = e->getCollider();
	if (obj->isSolid())
	{
		this->destroy();

	}
}

const Sprite & Projectile::getSprite() const
{
	return _sprite;
}

void Projectile::setSprite(const Sprite& sprite)
{
	_sprite = sprite;
}