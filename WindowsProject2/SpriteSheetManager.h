#pragma once
#include "Sprite.h"
#include "Types.h"
#include "tmxlite\Tileset.hpp"

#include "GMap.h"
#include "DynamicList.h"


class RenderEngine;
class SpriteSheet;


// this thing should manage all of the information that the map is loading regarding sprites
class SpriteSheetManager
{
public:
	SpriteSheetManager();
	~SpriteSheetManager();
	
	// TODO: Rename this..
	void doThing(const std::vector<tmx::Tileset>& tilesets, RenderEngine* render_engine);

	// ID from map goes in, sprite thing generated and comes out (returns null sprite if something goes wrong)
	Sprite getSpriteFromID(int id);
	DynamicList<RECTF_TYPE> getHitBoxesFromID(int id);

private:
	RECTF_TYPE getRectF(const tmx::FloatRect& rect);

	GMap<int, DynamicList<RECTF_TYPE>> _tile_collision_walls;
	DynamicList<SpriteSheet*> _sprite_sheets;
};






