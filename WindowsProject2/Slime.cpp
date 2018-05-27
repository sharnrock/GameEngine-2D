#include "Slime.h"
#include "Possessor.h"
#include "ControlEvent.h"


Slime::Slime(float x, float y, float w, float h) :
	DisplayableBitmap(x,y,w,h),
	_possessor(nullptr),
	speed_px_s(30),
	_acceleration(2.0)
{
	_obj_type = "Slime";
}

Slime::~Slime()
{
}

void Slime::moveLeft(__int64 dt)
{
	_move_force.x = -1 * _acceleration;
	//_x -= speed_px_s * dt / 1E6f;
	updateBoundingRect();
}

void Slime::moveUp(__int64 dt)
{
	_move_force.y = -1 * _acceleration;
	//_y -= speed_px_s * dt / 1E6f;
	updateBoundingRect();
}

void Slime::moveRight(__int64 dt)
{
	_move_force.x = 1 * _acceleration;
	//_x += speed_px_s * dt / 1E6f;
	updateBoundingRect();
}

void Slime::moveDown(__int64 dt)
{
	_move_force.y = 1 * _acceleration;
	//_y += speed_px_s * dt / 1E6f;
	updateBoundingRect();
}

void Slime::setPossesor(Possessor* possessor)
{
	_possessor = possessor;
	_possessor->setControllable(this);
}

void Slime::update(__int64 dt)
{
	_move_force.SetZero();
	_possessor->update(dt);
}

//const Sprite & Slime::getSprite() const
//{
//	return _sprite;
//}
//
//void Slime::setSprite(const Sprite& sprite)
//{
//	_sprite = sprite;
//	float width = _sprite.getSourceRect().right - _sprite.getSourceRect().left;
//	float height = _sprite.getSourceRect().bottom - _sprite.getSourceRect().top;
//	setSize(width, height);
//}

void Slime::onCollisionEvent(CollisionEvent* e)
{
	OutputDebugString(TEXT("Something hit slime\n"));
}

void Slime::onControlEvent(ControlEvent* e)
{
	switch (e->getControl())
	{
	case ControlEvent::MoveLeft:
		moveLeft(e->getDt());
		break;
	case ControlEvent::MoveRight:
		moveRight(e->getDt());
		break;
	case ControlEvent::MoveUp:
		moveUp(e->getDt());
		break;
	case ControlEvent::MoveDown:
		moveDown(e->getDt());
		break;
	}
	// Doesn't do anything else yet..
}
