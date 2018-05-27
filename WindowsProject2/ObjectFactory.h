#pragma once

#include "DynamicList.h"
class GameObject;
class Projectile;
class GameLoop;
class GraphicsEngine;
class SpriteSheetManager;
class AudioEngine;
class PhysicsEngine;



// Object pool for all the different stuff
// Not actually an object pool yet..
class ObjectFactory
{
public:
	~ObjectFactory();

	// Get singleton instance
	static ObjectFactory& Instance();

	// This must be called before creating any objects
	void initialize(GameLoop* gloop, GraphicsEngine* rengine, SpriteSheetManager* sprite_manager, AudioEngine* audio_engine, PhysicsEngine* physics_engine);

	// These don't do anything yet
	void releaseObject(GameObject* object);
	void releaseAllObjects() {} // TODO: implement this eventually

	void releaseProjectile(Projectile*);

	// Converts screen coords at target_x/y to world coordinates
	Projectile* createProjectile(float x, float y, float target_x, float target_y);
	GameObject* createRobot(float x, float y, int layer);
	GameObject* createSlime(float x, float y, int layer);
	GameObject* createBackgroundTile(float x, float y, int id, int layer);
	GameObject* createSpinnyBox(float x, float y, int id, int layer);

private:
	void loadUpGameObjectWithPtrs(GameObject * obj);

	GameLoop*                _game_loop;
	GraphicsEngine*          _render_engine;
	SpriteSheetManager*      _sprite_manager;
	AudioEngine*             _audio_engine;
	PhysicsEngine*           _physics_engine;
	
	DynamicList<GameObject*> _objects;
	DynamicList<Projectile*> _active_projs;
	DynamicList<Projectile*> _idle_projs;

	// Singleton stuff..
private:
	ObjectFactory() :
		_game_loop(nullptr),
		_render_engine(nullptr),
		_sprite_manager(nullptr),
		_audio_engine(nullptr),
		_physics_engine(nullptr),
		_objects(256) // TODO: decide how to initialize the rest of the dynamic lists
	{
	}

	ObjectFactory(const ObjectFactory&)             = delete;
	ObjectFactory& operator=(const ObjectFactory&)  = delete;
	ObjectFactory(const ObjectFactory&&)            = delete;
	ObjectFactory& operator=(const ObjectFactory&&) = delete;
};

// Example code from box2d...
// This will be done eventually
//
//GameActor* actor = GameCreateActor();
//b2BodyDef bodyDef;
//bodyDef.userData = actor;
//actor->body = box2Dworld->CreateBody(&bodyDef);

// And then fixtures fit into this somehow?
