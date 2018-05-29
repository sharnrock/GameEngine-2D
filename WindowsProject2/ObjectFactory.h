#pragma once

#include "Types.h"
#include "DynamicList.h"

#include "GMap.h"
#include "GStack.h"
#include "GString.h"

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
	//void releaseParticle(GameObject*);
	//void releaseProjectile(Projectile*);

	// Converts screen coords at target_x/y to world coordinates
	Projectile* createProjectile(float x, float y, float target_x, float target_y);
	GameObject* createRobot(float x, float y, int layer);
	GameObject* createSlime(float x, float y, int layer);
	GameObject* createBackgroundTile(float x, float y, float w, float h, int id, int layer);
	GameObject* createSpinnyBox(float x, float y, int id, int layer);
	GameObject* createParticle(VECTORF location, VECTORF velocity, float angle, float angle_velocity, float sizes, float life_time);
	GameObject* createParticleEmitter(float x, float y);


	GameObject* createCamera();
private:
	void loadUpGameObjectWithPtrs(GameObject * obj);
	bool hasIdleEquivalent(const GString& obj_type);

	GameLoop*                _game_loop;
	GraphicsEngine*          _render_engine;
	SpriteSheetManager*      _sprite_manager;
	AudioEngine*             _audio_engine;
	PhysicsEngine*           _physics_engine;
	
	//DynamicList<GameObject*> _objects;
	//DynamicList<Projectile*> _active_projs;
	//DynamicList<Projectile*> _idle_projs;

	//DynamicList<GameObject*> _active_particles;
	//DynamicList<GameObject*> _idle_particles;

	// TODO: finish this
	DynamicList<GameObject*>           _all_objects;
	GMap<GString, GStack<GameObject*>> _idle_objects;

	


	// Singleton stuff..
private:
	ObjectFactory() :
		_game_loop(nullptr),
		_render_engine(nullptr),
		_sprite_manager(nullptr),
		_audio_engine(nullptr),
		_physics_engine(nullptr),
		_all_objects(4096) // TODO: figure out optimal initial size.
	{
	}

	ObjectFactory(const ObjectFactory&)             = delete;
	ObjectFactory& operator=(const ObjectFactory&)  = delete;
	ObjectFactory(const ObjectFactory&&)            = delete;
	ObjectFactory& operator=(const ObjectFactory&&) = delete;
};


