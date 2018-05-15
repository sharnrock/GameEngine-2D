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
class ObjectFactory;
class SpriteManager;

// Base class for just about everything in the game world
class GameObject
{
public:
	GameObject(float x, float y, float width, float height, AudioEngine* audio_engine = nullptr);  // TODO: pull out audio engine
	GameObject();
	~GameObject();

	// give access to audio engine for events
	void setAudioEngine(AudioEngine* audio_engine) { _audio_engine = audio_engine; }
	void setObjectFactory(ObjectFactory* obj_factory) { _obj_factory = obj_factory; }
	void setSpriteManager(SpriteManager* sprite_manager) { _sprite_manager = sprite_manager; }

	// if returns true, it won't render or update or collide
	virtual void setActive(bool is_active = true) { _is_active = is_active; }
	virtual bool isActive() const { return _is_active; }

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
	// get the collisions to create a response or do whatever..
	virtual void addTinyCollisionBoxesForStage2Detect(const RECTF_TYPE& rect);
	const DynamicList<RECTF_TYPE>& getFineCollisionBoxes() const { return _fine_collision_boxes; }

	// return true if it needs collision checking
	virtual bool isSolid() const { return _is_solid; }
	virtual void setSolid(bool is_solid) { _is_solid = is_solid; }


	// These need to go..
	bool hasCoarseCollisionWith(const GameObject& other) const;
	virtual bool hasFineCollisionWith(const GameObject& other) const;
protected:
	bool hasCollision(const RECTF_TYPE& rect1, const RECTF_TYPE& rect2) const;
public:
	// End of stuff that has to go
	
	// handles events
	virtual void onEvent(Event* e);

	
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


	
	void updateBoundingRect();
	
	RECTF_TYPE              _bounding_rect;
	DynamicList<RECTF_TYPE> _fine_collision_boxes;
	bool                    _is_solid;
	bool                    _is_active;

//private: // will make these private later..
	float _x, _y; // these should probably be private so whenever they change update bounding box has to be called. no forgetting it
	float _w, _h;

	AudioEngine*   getAudioEngine() { assert(_audio_engine); return _audio_engine; }
	ObjectFactory* getObjectFactory() { assert(_obj_factory); return _obj_factory; }
	SpriteManager* getSpriteManager() { assert(_sprite_manager);  return _sprite_manager; }

private:
	AudioEngine*   _audio_engine;
	ObjectFactory* _obj_factory;
	SpriteManager* _sprite_manager;
};

