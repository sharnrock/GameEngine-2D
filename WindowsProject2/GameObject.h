#pragma once

#include "Types.h"
#include "DynamicList.h"
#include "GString.h"

class Event;
class CollisionEvent;
class BirthEvent;
class DestroyEvent;
class ControlEvent;
class AnimationEvent;

class AudioEngine;
class ObjectFactory;
class SpriteSheetManager;
class Camera;

// Base class for just about everything in the game world
class GameObject
{
public:
	GameObject(float x, float y, float width, float height); 
	GameObject();
	~GameObject();

	void setAudioEngine   (AudioEngine* audio_engine)          { _audio_engine   = audio_engine;   }
	void setObjectFactory (ObjectFactory* obj_factory)         { _obj_factory    = obj_factory;    }
	void setSpriteManager (SpriteSheetManager* sprite_manager) { _sprite_manager = sprite_manager; }
	void setCamera        (Camera* camera)                     { _camera         = camera;         }
	
	// this should return a gstring type
	// TODO: can this become static, so you don't have to create GString("type") strings and store them everywhere..?
	virtual const GString& getObjectType() const { return _obj_type; }
	
	// if returns true, it won't render or update or collide
	virtual void setActive(bool is_active = true) { _is_active = is_active; }
	virtual bool isActive() const { return _is_active; }

	// similar to step event
	virtual void update(__int64 dt) {}

	// X,Y marks top left of object
	void  setWorldCoordinates(float x, float y); // updates the bounding rect
	float X() const { return _x; }
	float Y() const { return _y; }

	// These coordinates should make the center of the object
	virtual void setWorldCenterCoords(float x, float y);
	virtual VECTORF getWorldCenterCoords() const;

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

	// TODO: rename this to velocity .. or something that is true; it's not a force
	VECTORF getMoveForce() const { return _move_force; }

	// handles events
	virtual void onEvent(Event* e);


protected:
	VECTORF _move_force; // gonna try and use this for moving stuff within physics engine
	GString _obj_type;
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

	AudioEngine*        getAudioEngine() { assert(_audio_engine); return _audio_engine; }
	ObjectFactory*      getObjectFactory() { assert(_obj_factory); return _obj_factory; }
	SpriteSheetManager* getSpriteManager() { assert(_sprite_manager);  return _sprite_manager; }
	Camera*             getCamera() { return _camera; }

private:
	AudioEngine*        _audio_engine;
	ObjectFactory*      _obj_factory;
	SpriteSheetManager* _sprite_manager;
	Camera*             _camera;
};

