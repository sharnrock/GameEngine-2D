#pragma once

#include "DynamicList.h"
#include "GMap.h"
#include "ObjectFactory.h"

class GameLoop;
class RenderBullshit;
class HumanInputDeviceState;
class SpriteSheet;
class WallCollisions;

class LevelLoader
{
public:
	LevelLoader();
	~LevelLoader();

	void loadLevel();

	void setGameLoop(GameLoop* game_loop)                           { _game_loop    = game_loop; }
	void setRenderBullshit(RenderBullshit* renderer)                { _render_thing = renderer; }
	void setHumanInputDeviceState(HumanInputDeviceState* hid_state) { _hid_state    = hid_state; }

private:
	GameLoop*                  _game_loop;
	RenderBullshit*            _render_thing;
	HumanInputDeviceState*     _hid_state;
	DynamicList<SpriteSheet*>  _sheets;
	GMap<int, WallCollisions*> _wall_collisions;
	ObjectFactory _obj_factory;
};

