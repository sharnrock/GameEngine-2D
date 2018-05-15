#pragma once

#include "Displayable.h"
#include "AnimatedSprite.h"

class SpinnyBox :
	public DisplayableAnimation
{
public:
	SpinnyBox();
	~SpinnyBox();

	void update(__int64 dt);

	const Sprite& getSprite() const;
	void setSprite(const Sprite &sprite);

protected:
	virtual void onCollisionEvent(CollisionEvent*);
	virtual void onBirthEvent(BirthEvent*);
	virtual void onDestroyEvent(DestroyEvent*);
	virtual void onAnimationEvent(AnimationEvent*);
	virtual void onControlEvent(ControlEvent*);

private:
	AnimatedSprite _ani_sprite;
	Sprite _sprite;

	GString _sprite_location;
};

