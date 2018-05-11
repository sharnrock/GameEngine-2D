#pragma once

#include "DynamicList.h"
#include "GMap.h"
#include "ObjectFactory.h"
#include "SpriteSheetManager.h"

class GameLoop;
class RenderBullshit;
class HumanInputDeviceState;
class SpriteSheet;
class WallCollisions;

namespace tmx
{
	class TileLayer;
	class ObjectGroup;
}

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
	void createTileLayer(tmx::TileLayer* tile_layer, int layer);
	void createObjectLayer(tmx::ObjectGroup* obj_layer, int layer);

	GameLoop*                  _game_loop;
	RenderBullshit*            _render_thing;
	HumanInputDeviceState*     _hid_state;
	ObjectFactory              _obj_factory;
	SpriteSheetManager         _sprite_manager;
};

