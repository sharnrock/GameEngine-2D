#include "GameLoop.h"
#include "GameObject.h"
#include "CollisionEvent.h"
#include "LevelLoader.h"
#include "GraphicsEngine.h"
#include "PhysicsEngine.h"


GameLoop::GameLoop() :
	_updatables(1),
	level_loader(nullptr),
	graphics(nullptr),
	physics(nullptr) 
{
}

GameLoop::~GameLoop()
{
}

void GameLoop::update(__int64 delta_t_us)
{
	// Update game logic: actions like AI, keyboard strokes, timers etc..
	updateObjects(delta_t_us);

	// update collisions, then update physics and position
	updateCollisions(delta_t_us);

	// Draw to screen
	graphics->OnRender();
}

void GameLoop::updateObjects(__int64 dt)
{
	for (int i = 0; i < _updatables.count(); i++)
	{
		if (_updatables[i]->isActive())
			_updatables[i]->update(dt);
	}
}

void GameLoop::updateCollisions(__int64 delta_t_us)
{
	physics->update(delta_t_us);

}

void GameLoop::addUpdatableObject(GameObject* updatable_object)
{
	_updatables.append(updatable_object);
}

HRESULT GameLoop::initialize()
{
	loadLevel();
	physics->initialize();
	return S_OK;
}

HRESULT GameLoop::uninitialize()
{
	return S_OK;
}

void GameLoop::loadLevel()
{
	assert(this->level_loader);
	this->level_loader->loadLevel();
}
