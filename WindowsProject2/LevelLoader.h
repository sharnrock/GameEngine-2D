#pragma once

#include "ObjectFactory.h"
#include "SpriteSheetManager.h"

class AudioEngine;
class GameLoop;
class GraphicsEngine;
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
	void setRenderBullshit(GraphicsEngine* renderer)                { _render_thing = renderer; }
	void setHumanInputDeviceState(const HumanInputDeviceState* hid_state) { _hid_state    = hid_state; }

private:
	void createTileLayer(tmx::TileLayer* tile_layer, int layer);
	void createObjectLayer(tmx::ObjectGroup* obj_layer, int layer);

	GameLoop*                    _game_loop;
	GraphicsEngine*              _render_thing;
	const HumanInputDeviceState* _hid_state;
	SpriteSheetManager           _sprite_manager;
	AudioEngine*                 _audio_engine;
};

