#pragma once
#include "Displayable.h"
#include "Sprite.h"

class ObjectFactory;
class ControlEvent;
class Possessor;

class Robot :
	public DisplayableBitmap
{
public:
	Robot(float x, float y, float w, float h, ObjectFactory* obj_factory);
	~Robot();

	void setPossesor(Possessor* possessor);
	void update(__int64 dt);

	//const Sprite & getSprite() const;
	//void setSprite(const Sprite& sprite);

	bool isSolid() const { return true; }

protected:
	void onCollisionEvent(CollisionEvent* e);
	void onControlEvent(ControlEvent* e);

private:
	void moveLeft(__int64 dt);
	void moveUp(__int64 dt);
	void moveRight(__int64 dt);
	void moveDown(__int64 dt);
	void firePrimary(__int64 dt);

	Possessor* _possessor;
	float speed_px_s;
	/*Sprite _sprite;*/
	ObjectFactory* _obj_factory;
	__int64 _weapon_timer;
	__int64 _weapon_cooldown;
};

