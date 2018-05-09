#pragma once

#include "Displayable.h"
#include "Sprite.h"

class Projectile :
	public DisplayableBitmap
{
public:
	Projectile(float x, float y, float target_x, float target_y);
	~Projectile();

	void setSpeed(float speed);
	void setTarget(float x, float y);
	void setAngle(float degrees);
	void update(__int64 dt);

	const Sprite & getSprite() const;
	void setSprite(const Sprite& sprite);

protected:
	void onCollisionEvent(CollisionEvent* e);

private:
	void updateAngle();

	float _speed;
	float _target_x, _target_y;
	float _angle_dgr;

	// Limit the life of the projectile
	float _max_distance;
	float _distance_travelled; 

	Sprite _sprite;

};

