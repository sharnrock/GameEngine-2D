#pragma once

#include "DynamicList.h"
#include "MainLoop.h"

class Updatable;
class GameObject;
class LevelLoader;
class GraphicsEngine;
class PhysicsEngine;

class GameLoop :
	public MainLoop
{
public:
	GameLoop();
	~GameLoop();

	HRESULT initialize();
	HRESULT uninitialize();
	void    loadLevel();

	void setLevelLoader(LevelLoader* level_loader)   { this->level_loader = level_loader; }
	void setGraphicsEngine(GraphicsEngine* graphics) { this->graphics = graphics; }
	void setPhysicsEngine(PhysicsEngine* physics)    { this->physics = physics; }

	void update(__int64 delta_t_us);
	void addUpdatableObject(GameObject* updatable_object);

private:
	void updateCollisions(__int64 delta_t_us);
	void updateObjects(__int64 dt);

	DynamicList<GameObject*> _updatables;
	LevelLoader*             level_loader;
	GraphicsEngine*          graphics;
	PhysicsEngine*           physics;
};

