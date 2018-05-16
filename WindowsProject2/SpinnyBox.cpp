#include "SpinnyBox.h"
#include "SpriteSheetManager.h"

SpinnyBox::SpinnyBox() :
	_sprite_location("E:\\Keith\\Documents\\Sprites\\cube.png")
{
}


SpinnyBox::~SpinnyBox()
{
}

void SpinnyBox::update(__int64 dt)
{
	_ani_sprite.update(dt);
}

const Sprite & SpinnyBox::getSprite() const
{
	return _ani_sprite.generateSpriteFromCurrentFrame();
}

void SpinnyBox::setSprite(const Sprite &sprite)
{
	_sprite = sprite;
	float width = _sprite.getSourceRect().right - _sprite.getSourceRect().left;
	float height = _sprite.getSourceRect().bottom - _sprite.getSourceRect().top;
	setSize(width, height);
}

void SpinnyBox::onCollisionEvent(CollisionEvent*)
{
}

void SpinnyBox::onBirthEvent(BirthEvent*)
{
	_ani_sprite = getSpriteManager()->getAnimatedSprite(_sprite_location);
	_ani_sprite.sendEventsToGameObject(this);
}

void SpinnyBox::onDestroyEvent(DestroyEvent*)
{
}

void SpinnyBox::onAnimationEvent(AnimationEvent*)
{
}

void SpinnyBox::onControlEvent(ControlEvent*)
{
}