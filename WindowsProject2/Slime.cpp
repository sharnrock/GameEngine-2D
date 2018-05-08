#include "Slime.h"
#include "Possessor.h"


Slime::Slime(float x, float y, float w, float h) :
	DisplayableBitmap(x,y,w,h),
	_possessor(nullptr),
	speed_px_s(30)
{
}

Slime::~Slime()
{
}

void Slime::moveLeft(__int64 dt)
{
	_x -= speed_px_s * dt / 1E6f;
	updateBoundingRect();
}

void Slime::moveUp(__int64 dt)
{
	_y -= speed_px_s * dt / 1E6f;
	updateBoundingRect();
}

void Slime::moveRight(__int64 dt)
{
	_x += speed_px_s * dt / 1E6f;
	updateBoundingRect();
}

void Slime::moveDown(__int64 dt)
{
	_y += speed_px_s * dt / 1E6f;
	updateBoundingRect();
}

void Slime::setPossesor(Possessor* possessor)
{
	_possessor = possessor;
	_possessor->setControllable(this);
}

void Slime::update(__int64 dt)
{
	_possessor->update(dt);
}

const Sprite & Slime::getSprite() const
{
	return _sprite;
}

void Slime::setSprite(const Sprite& sprite)
{
	_sprite = sprite;
	float width = _sprite.getSourceRect().right - _sprite.getSourceRect().left;
	float height = _sprite.getSourceRect().bottom - _sprite.getSourceRect().top;
	setSize(width, height);
}

void Slime::onCollisionEvent(CollisionEvent* e)
{
	OutputDebugString(TEXT("Something hit slime\n"));
}
