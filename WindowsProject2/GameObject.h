#pragma once

#include "Types.h"
#include "DynamicList.h"

class Event;
class CollisionEvent;
class BirthEvent;
class DestroyEvent;
class ControlEvent;
class AnimationEvent;

class AudioEngine;

// Base class for just about everything in the game world
class GameObject
{
public:
	GameObject(float x, float y, float width, float height, AudioEngine* audio_engine = nullptr);
	GameObject();
	~GameObject();

	// similar to step event
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

	// if returns true, it won't render or update or collide
	virtual void setActive(bool is_active = true) { _is_active = is_active; }
	virtual bool isActive() const {	return _is_active; }

	void setAudioEngine(AudioEngine* audio_engine) { _audio_engine = audio_engine; }

protected:

	// Events
	
	// ran into something
	virtual void onCollisionEvent(CollisionEvent*) {}

	// the object has become active or instantiated 
	virtual void onBirthEvent(BirthEvent*) {}

	// time to put the object away..
	virtual void onDestroyEvent(DestroyEvent*) {}

	// the animation has started or finished or something
	virtual void onAnimationEvent(AnimationEvent*) {}

	// A control action has been performed; could have been keyboard or AI control..
	virtual void onControlEvent(ControlEvent*) {}


	bool hasCollision(const RECTF_TYPE& rect1, const RECTF_TYPE& rect2) const;
	void updateBoundingRect();
	
	RECTF_TYPE _bounding_rect;
	bool _is_solid;
	DynamicList<RECTF_TYPE> _fine_collision_boxes;
	bool _is_active;

//private: // will make these private later..
	float _x, _y; // these should probably be private so whenever they change update bounding box has to be called. no forgetting it
	float _w, _h;

	AudioEngine* getAudioEngine() { return _audio_engine; }
private:
	AudioEngine* _audio_engine;
};

