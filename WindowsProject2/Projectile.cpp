#include "Projectile.h"
#include "CollisionEvent.h"

#include "Locations.h"
#include "AudioEngine.h"
#include <assert.h>

Projectile::Projectile(float x, float y, float target_x, float target_y) :
	DisplayableBitmap(x, y, 0, 0),
	_target_x(target_x),
	_target_y(target_y),
	_speed(400),
	_distance_travelled(0),
	_fire_sound(AUDIO_PATH "shoot.wav")
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
	float distance = (float)(dt * _speed / 1E6f);
	_x += distance;
	_distance_travelled += distance;
	if (_distance_travelled > _max_distance)
		destroy();
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

void Projectile::onBirthEvent(BirthEvent*)
{
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