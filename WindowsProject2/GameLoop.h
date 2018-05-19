#pragma once

#include "DynamicList.h"
#include "MainLoop.h"

class Updatable;
class GameObject;
class LevelLoader;
class GraphicsEngine;

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
	void loadLevel(); // TEMP TODO:
	void update(__int64 delta_t_us);

	// This creates N^2 time... we'll fix this soon
	// IT's probably best to create a collision engine portion that'll update separate
	void updateCollisions(__int64 delta_t_us);
	void addUpdatableObject(GameObject* updatable_object);

private:
	DynamicList<GameObject*> _updatables;
	DynamicList<GameObject*> _collideables;
	LevelLoader* level_loader;
	GraphicsEngine* graphics;


		// Inherited via MainLoop

	// Inherited via MainLoop
};

