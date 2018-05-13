#pragma once

//#include "Sprite.h"
//#include "Types.h"
//#include "ObjectDefines.h"
#include "DynamicList.h"

class GameObject;
class Projectile;
class GameLoop;
class RenderEngine;
class SpriteSheetManager;
class AudioEngine;

// Object pool for all the different stuff
// Not actually an object pool yet..
class ObjectFactory
{
public:
	ObjectFactory(GameLoop* gloop, RenderEngine* rengine, SpriteSheetManager* sprite_manager, AudioEngine* audio_engine);
	~ObjectFactory();

	// copies & moves
	ObjectFactory(const ObjectFactory&)             = delete;
	ObjectFactory(const ObjectFactory&&)            = delete;
	ObjectFactory& operator=(const ObjectFactory&)  = delete;
	ObjectFactory& operator=(const ObjectFactory&&) = delete;

	void setGameLoop(GameLoop* gloop) { _game_loop = gloop; }
	void setRenderEngine(RenderEngine* rengine) { _render_engine = rengine; }
	void setSpriteManager(SpriteSheetManager* sprite_manager) { _sprite_manager = sprite_manager; }
	void setAudioEngine(AudioEngine* audio_engine) { _audio_engine = audio_engine; }

	// These don't do anything yet
	void releaseObject(GameObject* object);
	void releaseAllObjects() {} // TODO: implement this eventually

	// These are actually used
	Projectile* createProjectile(float x, float y, float target_x, float target_y);
	GameObject* createRobot(float x, float y, int layer);
	GameObject* createSlime(float x, float y, int layer);
	GameObject* createBackgroundTile(float x, float y, int id, int layer);

private:
	GameLoop*                _game_loop;
	RenderEngine*            _render_engine;
	SpriteSheetManager*      _sprite_manager;
	DynamicList<GameObject*> _objects;
	AudioEngine* _audio_engine;
};

