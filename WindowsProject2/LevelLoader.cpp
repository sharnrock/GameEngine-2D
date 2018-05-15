
// TODO: go through these and get rid of what's not needed
#include "LevelLoader.h"
#include "GameLoop.h"
#include "RenderBullshit.h"
#include "SpriteSheetManager.h"
#include "Robot.h"
#include "Slime.h"
#include "SpinnyBox.h"
#include "FPSCounter.h"
#include "Camera.h"
#include "AIPossessor.h"
#include "HumanPossessor.h"
#include "BackGroundTileDisplayable.h"
#include "SpriteSheet.h"
#include "GString.h"

#include "Locations.h"

#include "AudioEngineFactory.h"
#include "AudioEngine.h"

// Map lib
#include "tmxlite\Map.hpp"
#include "tmxlite\Tileset.hpp"
#include "tmxlite\TileLayer.hpp"

#include <assert.h>
#include <vector>

// temp for test..
#include "GDir.h"
#include "DynamicList.h"

LevelLoader::LevelLoader() :
	_game_loop(nullptr),
	_render_thing(nullptr),
	_hid_state(nullptr),
	_audio_engine(nullptr)
{
}

LevelLoader::~LevelLoader()
{
}

void LevelLoader::loadLevel()
{
	assert(_game_loop);
	assert(_render_thing);
	assert(_hid_state);

	_audio_engine = AudioEngineFactory::getAudioEngine();
	_audio_engine->init();
	_audio_engine->loadFilesInThisDir(AUDIO_PATH);
	ObjectFactory::Instance().initialize(_game_loop, _render_thing, &_sprite_manager, _audio_engine);


	_audio_engine->playSound(AUDIO_PATH "jump.wav");
	


	tmx::Map map;
	if (!map.load("E:\\Keith\\Documents\\TheBigGame\\Mapping\\images\\map3.tmx"))
	{
		OutputDebugString(TEXT("Couldn't load tmx map\n"));
		return;
	}
	OutputDebugString(TEXT("Loaded tmx map\n"));

	std::vector<tmx::Tileset> tilesets = map.getTilesets();
	if (tilesets.empty())
		return;

	// Load up all the images
	_sprite_manager.setRenderEngine(_render_thing);
	_sprite_manager.loadBitMapsForTileMap(tilesets);
		
	// Go through all the layers and draw them
	const std::vector<tmx::Layer::Ptr>& layers = map.getLayers();
	for (int layer_i = 0; layer_i < layers.size(); layer_i++)
	{
		// Load up tile layers
		if (layers[layer_i]->getType() == tmx::Layer::Type::Tile)
		{
			tmx::Layer* layer = layers[layer_i].get();
			tmx::TileLayer* tile_layer = static_cast<tmx::TileLayer*>(layer);
			createTileLayer(tile_layer, layer_i);
		}

		// Load up object layers where all the players and items are
		if (layers[layer_i]->getType() == tmx::Layer::Type::Object)
		{
			tmx::Layer* layer = layers[layer_i].get();
			tmx::ObjectGroup* obj_layer = static_cast<tmx::ObjectGroup*>(layer);
			createObjectLayer(obj_layer, layer_i);
		}
	}

	// Set up fps counter
	FPSCounter* fps_counter = new FPSCounter(0, 0, 100, 10);
	_render_thing->addDisplayableObject(fps_counter);
	_game_loop->addUpdatableObject(fps_counter);
}

void LevelLoader::createTileLayer(tmx::TileLayer* tile_layer, int layer)
{
	const std::vector<tmx::TileLayer::Tile> tiles = tile_layer->getTiles();
	for (int i = 0; i < tiles.size(); i++)
	{
		int id = tiles.at(i).ID;
		if (!id)
			continue;

		int tile_count_in_width = (int)tile_layer->getWidth();

		float x = (float)((int)(i % tile_count_in_width)) * _sprite_manager.getSpriteFromID(id).getWidth();
		float y = (float)((int)(i / tile_count_in_width)) * _sprite_manager.getSpriteFromID(id).getHeight();

		ObjectFactory::Instance().createBackgroundTile(x, y, id, layer);
	}
}

void LevelLoader::createObjectLayer(tmx::ObjectGroup* obj_layer, int layer)
{
	const std::vector<tmx::Object>& objects = obj_layer->getObjects();
	for (int i = 0; i < objects.size(); ++i)
	{
		// create the player stuff
		if (objects.at(i).getType() == "Robot")
		{
			float x = objects.at(i).getPosition().x;
			float y = objects.at(i).getPosition().y;

			Possessor* poss = new HumanPossessor(_hid_state);
			Robot* robot = static_cast<Robot*>(ObjectFactory::Instance().createRobot(x, y, layer));

			robot->setPossesor(poss);
			poss->setControllable(robot);
		}
		else if (objects.at(i).getType() == "Slime")
		{
			float x = objects.at(i).getPosition().x;
			float y = objects.at(i).getPosition().y;
			Slime *thing = static_cast<Slime*>(ObjectFactory::Instance().createSlime(x, y, layer));

			Possessor* poss = new AIPossessor();
			thing->setPossesor(poss);
			poss->setControllable(thing);
		}
		else if (objects.at(i).getType() == "SpinnyBox")
		{
			float x = objects.at(i).getPosition().x;
			float y = objects.at(i).getPosition().y;
			
			SpinnyBox* thing = static_cast<SpinnyBox*>(ObjectFactory::Instance().createSpinnyBox(x, y, 43, layer));
		}
	}
}