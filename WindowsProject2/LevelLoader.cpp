
// TODO: go through these and get rid of what's not needed
#include "LevelLoader.h"
#include "GameLoop.h"
#include "RenderBullshit.h"
#include "DisplayableThing.h"
#include "AIPossessor.h"
#include <assert.h>
#include "tmxlite\Map.hpp"
#include "tmxlite\Tileset.hpp"
#include "tmxlite\TileLayer.hpp"

#include "Robot.h"
#include "Slime.h"
#include "SpriteSheetManager.h"
#include "FPSCounter.h"
#include "Camera.h"
#include "HumanPossessor.h"
#include "BackGroundTileDisplayable.h"
#include "SpriteSheet.h"
#include "GString.h"

#include "Locations.h"
// this might change..
#include "AudioEngineFactory.h"
#include "AudioEngine.h"

// These are temporary just to test if box2d is grabbed correctly
#include "Box2D\Box2D.h"


#include <vector>

LevelLoader::LevelLoader() :
	_game_loop(nullptr),
	_render_thing(nullptr),
	_hid_state(nullptr),
	_obj_factory(_game_loop, _render_thing, &_sprite_manager, nullptr),
	_audio_engine(nullptr)
{
}

LevelLoader::~LevelLoader()
{
}

void LevelLoader::loadLevel()
{
	// physics testing.. remove eventually
	b2Vec2 gravity(0.0f, -10.0f);
	b2World world(gravity);

	assert(_game_loop);
	assert(_render_thing);
	assert(_hid_state);

	_audio_engine = AudioEngineFactory::getAudioEngine();
	_audio_engine->init();
	_audio_engine->loadTestSound();
	_audio_engine->playTestSound();
	// _audio_engine->loadFilesInThisDir(AUDIO_PATH); // doesn't do anything yet..

	// This is probably temporary....
	// TODO: The constructor should likely just take the dep inj and give it so these don't have to be called
	_obj_factory.setGameLoop(_game_loop);
	_obj_factory.setRenderEngine(_render_thing);
	_obj_factory.setSpriteManager(&_sprite_manager);
	_obj_factory.setAudioEngine(_audio_engine);

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

		_obj_factory.createBackgroundTile(x, y, id, layer);
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
			Robot* robot = static_cast<Robot*>(_obj_factory.createRobot(x, y, layer));

			robot->setPossesor(poss);
			poss->setControllable(robot);
		}
		else if (objects.at(i).getType() == "Slime")
		{
			float x = objects.at(i).getPosition().x;
			float y = objects.at(i).getPosition().y;
			Slime *thing = static_cast<Slime*>(_obj_factory.createSlime(x, y, layer));

			Possessor* poss = new AIPossessor();
			thing->setPossesor(poss);
			poss->setControllable(thing);
		}
	}
}