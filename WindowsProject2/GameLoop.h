#pragma once

#include "DynamicList.h"
#include "MainLoop.h"

class Updatable;
class GameObject;
class LevelLoader;
class GraphicsEngine;

// TODO: this needs to be replaced by interface 'PhysicsEngine'
//class Box2dPhysicsEngine;
//#include "Box2dPhysicsEngine.h"
#include "PhysicsEngine.h"

class GameLoop :
	public MainLoop
{
public:
	GameLoop();
	~GameLoop();

	HRESULT initialize();
	HRESULT uninitialize();

	void setLevelLoader(LevelLoader* level_loader) { this->level_loader = level_loader; }
	void setGraphicsEngine(GraphicsEngine* graphics) { this->graphics = graphics; }
	void setPhysicsEngine(PhysicsEngine* physics) { this->physics = physics; }
	void loadLevel(); // TEMP TODO:
	void update(__int64 delta_t_us);

	// This creates N^2 time... we'll fix this soon
	// IT's probably best to create a collision engine portion that'll update separate
	void updateCollisions(__int64 delta_t_us);
	void updateObjects(__int64 dt);
	void addUpdatableObject(GameObject* updatable_object);

private:
	DynamicList<GameObject*> _updatables;
	DynamicList<GameObject*> _collideables;
	LevelLoader*             level_loader;
	GraphicsEngine*          graphics;

	// This is concrete instance.. it needs to be pulled out in place of interface
	// TODO: just for development
	PhysicsEngine*      physics;
};

