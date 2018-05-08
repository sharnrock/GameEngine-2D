#pragma once

#include "Updatable.h"
#include "Types.h"
#include "DynamicList.h"

class Event;
class MoveEvent;
class CollisionEvent;

class GameObject :
	public Updatable
{
public:
	GameObject(float x, float y, float width, float height);
	GameObject();
	~GameObject();

	virtual void update(__int64 dt) {}

	// X,Y marks top left of object
	void  setWorldCoordinates(float x, float y); // updates the bounding rect
	float X() const { return _x; }
	float Y() const { return _y; }
	
	void  setSize(float width, float height);    // updates the bounding rect
	float getHeight() const { return _h; }
	float getWidth() const { return _w; }

	virtual const RECTF_TYPE& getBoundingRect() const;

	// return true if it needs collision checking
	virtual bool isSolid() const { return _is_solid; }
	virtual void setSolid(bool is_solid) { _is_solid = is_solid; }

	virtual void addTinyCollisionBoxesForStage2Detect(const RECTF_TYPE& rect);

	// Detects if the bounding rects are intersecting
	bool hasCoarseCollisionWith(const GameObject& other) const;

	// Use this to create a second stage of collision detection for file details
	// Default just returns the answer to coarse collision
	virtual bool hasFineCollisionWith(const GameObject& other) const;

	// get the collisions to create a response or do whatever..
	const DynamicList<RECTF_TYPE>& getFineCollisionBoxes() const { return _fine_collision_boxes; }
	
	// handles events
	virtual void onEvent(Event* e);

protected:

	// Events
	virtual void onMoveEvent(MoveEvent* e) {}
	virtual void onCollisionEvent(CollisionEvent* e) {}
	

	bool hasCollision(const RECTF_TYPE& rect1, const RECTF_TYPE& rect2) const;

	void updateBoundingRect();
	float _x, _y; // these should probably be private so whenever they change update bounding box has to be called. no forgetting it
	float _w, _h;
	RECTF_TYPE _bounding_rect;
	bool _is_solid;

	DynamicList<RECTF_TYPE> _fine_collision_boxes;
};

