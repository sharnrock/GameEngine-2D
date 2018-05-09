#pragma once

#include "Sprite.h"
#include "Types.h"

class Projectile;
class GameLoop;
class RenderEngine;

// Object pool for all the different stuff
class ObjectFactory
{
public:
	ObjectFactory(GameLoop* gloop, RenderEngine* rengine);
	~ObjectFactory();

	void setGameLoop(GameLoop* gloop) { _game_loop = gloop; }
	void setRenderEngine(RenderEngine* rengine) { _render_engine = rengine; }

	void setCollisionForProjectile(const RECTF_TYPE& collision);

	void setSpriteForProjectile(const Sprite& sprite);
	Projectile* createProjectile(float x, float y, float target_x, float target_y);

private:
	Sprite _projectile_sprite;
	GameLoop* _game_loop;
	RenderEngine* _render_engine;
	RECTF_TYPE _collision;
};

