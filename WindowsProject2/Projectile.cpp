#include "Projectile.h"
#include "CollisionEvent.h"

#include "Locations.h"
#include "AudioEngine.h"
#include "ObjectFactory.h"

#include <math.h>
#include <assert.h>

Projectile::Projectile(float x, float y, float target_x, float target_y) :
	DisplayableBitmap(x, y, 0, 0),
	_target_x(target_x),
	_target_y(target_y),
	_speed(400),
	_distance_travelled(0),
	_fire_sound(AUDIO_PATH "shoot.wav"),
	_max_distance(400), 
	_angle_rad(0)
{
	_obj_type = "Projectile";
	
	_delta_x = _target_x - _x;
	_delta_y = _target_y - _y;

	updateAngle();
	_w = 32;
	_h = 32;
	updateBoundingRect();
}

void Projectile::updateAngle()
{
	_angle_rad = atan2f(_delta_y, _delta_x);
}


Projectile::~Projectile()
{
}

void Projectile::destroy()
{
	getObjectFactory()->releaseProjectile(this);
}

void Projectile::setSpeed(float speed)
{
	_speed = speed;
}

void Projectile::setTarget(float x, float y)
{
	_target_x = x;
	_target_y = y;
	_delta_x = _target_x - _x;
	_delta_y = _target_y - _y;
	updateAngle();
}

void Projectile::setAngle(float rads)
{
	_angle_rad = rads;
}

void Projectile::update(__int64 dt)
{
	// TODO: put angle into this calc!
	float distance = (float)(dt * _speed / 1E6f);

	_x += distance * cosf(_angle_rad);
	_y += distance * sinf(_angle_rad);

	_distance_travelled += distance;
	if (_distance_travelled > _max_distance)
		destroy();
	updateBoundingRect();
}

void Projectile::onCollisionEvent(CollisionEvent* e)
{
	static GString robot_type("Robot");
	if (e->getCollider()->getObjectType() == robot_type)
		return;

	GameObject* obj = e->getCollider();
	if (obj->isSolid())
	{
		this->destroy();
	}
}

void Projectile::onBirthEvent(BirthEvent*)
{
	_distance_travelled = 0;
	getAudioEngine()->playSound(_fire_sound);
}

const Sprite & Projectile::getSprite() const
{
	return _sprite;
}

void Projectile::setSprite(const Sprite& sprite)
{
	_sprite = sprite;
}