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

	//const Sprite & getSprite() const;
	//void setSprite(const Sprite& sprite);

	bool isSolid() const { return true; }

protected:
	void onCollisionEvent(CollisionEvent* e);
	void onControlEvent(ControlEvent* e);
	
	void moveLeft(__int64 dt);
	void moveUp(__int64 dt);
	void moveRight(__int64 dt);
	void moveDown(__int64 dt);

private:
	Possessor * _possessor;
	float speed_px_s;
	//Sprite _sprite;
};

