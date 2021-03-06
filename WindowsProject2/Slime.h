#pragma once

#include "Displayable.h"
#include "Sprite.h"

class Possessor;

class Slime :
	public DisplayableBitmap
{
public:
	Slime(float x, float y, float w, float h);
	~Slime();


	void setPossesor(Possessor* possessor);

	void update(__int64 dt);

	float getDamage() const { return _damage; }

	bool isSolid() const { return true; }

protected:
	void onCollisionEvent(CollisionEvent* e);
	void onControlEvent(ControlEvent* e);
	
	void moveLeft(__int64 dt);
	void moveUp(__int64 dt);
	void moveRight(__int64 dt);
	void moveDown(__int64 dt);

private:
	float _health;
	float _damage;

	Possessor * _possessor;
	float speed_px_s;
	float _acceleration;
};

