#include "GameObject.h"
#include "Event.h"
#include "CollisionEvent.h"
#include "ControlEvent.h"
#include "AnimationEvent.h"
#include "DestroyEvent.h"
#include "BirthEvent.h"

GameObject::GameObject() :
	_x(0),
	_y(0),
	_w(0),
	_h(0),
	_is_solid(false),
	_is_active(true),
	_audio_engine(nullptr),
	_sprite_manager(nullptr)
{
	_bounding_rect.left   = 0;
	_bounding_rect.right  = 0;
	_bounding_rect.top    = 0;
	_bounding_rect.bottom = 0;
}

GameObject::GameObject(float x, float y, float width, float height, AudioEngine* audio_engine) :
	_x(x),
	_y(y),
	_w(width),
	_h(height),
	_is_solid(false),
	_is_active(true),
	_audio_engine(audio_engine),
	_obj_factory(nullptr),
	_sprite_manager(nullptr),
	_camera(nullptr)
{
	updateBoundingRect();
}

GameObject::~GameObject()
{
}

void GameObject::setWorldCoordinates(float x, float y)
{
	_x = x;
	_y = y;
	updateBoundingRect();
}

void GameObject::setSize(float width, float height)
{
	_w = width;
	_h = height;
	updateBoundingRect();
}

const RECTF_TYPE& GameObject::getBoundingRect() const
{ 
	return _bounding_rect; 
}

void GameObject::updateBoundingRect()
{
	_bounding_rect.left = _x;
	_bounding_rect.right = _x + _w;
	_bounding_rect.top = _y;
	_bounding_rect.bottom = _y + _h;
}

bool GameObject::hasCoarseCollisionWith(const GameObject& other) const
{
	const RECTF_TYPE &rect1 = this->getBoundingRect();
	const RECTF_TYPE &rect2 = other.getBoundingRect();
	return hasCollision(rect1, rect2);
}

bool GameObject::hasFineCollisionWith(const GameObject& other) const
{
	// This is going to require everything to have a defined collision per tile
	for (int y = 0; y < other._fine_collision_boxes.count(); y++)
	{			
		RECTF_TYPE rect2 = other._fine_collision_boxes.at(y);
		rect2.left += other._x;
		rect2.right += other._x;
		rect2.top += other._y;
		rect2.bottom += other._y;

		for (int i = 0; i < _fine_collision_boxes.count(); i++)
		{
			RECTF_TYPE rect1 = _fine_collision_boxes.at(i);

			rect1.left += _x;
			rect1.right += _x;
			rect1.top += _y;
			rect1.bottom += _y;

			if (hasCollision(rect1, rect2))
				return true;
		}
	}
	return false;
}

bool GameObject::hasCollision(const RECTF_TYPE& rect1, const RECTF_TYPE& rect2) const
{
	return (rect1.left < rect2.right &&
		rect1.right > rect2.left &&
		rect1.top < rect2.bottom &&
		rect1.bottom > rect2.top);
}

void GameObject::onEvent(Event* e)
{
	switch (e->getType())
	{
	case Event::Control:
		onControlEvent(static_cast<ControlEvent*>(e));
		break;
	case Event::Collision:
		onCollisionEvent(static_cast<CollisionEvent*>(e));
		break;
	case Event::Animation:
		onAnimationEvent(nullptr);
		break;
	case Event::Destroy:
		onDestroyEvent(nullptr);
		break;
	case Event::Birth:
		onBirthEvent(nullptr);
		break;
	}
}

void GameObject::addTinyCollisionBoxesForStage2Detect(const RECTF_TYPE& rect)
{
	_fine_collision_boxes.append(rect);
}