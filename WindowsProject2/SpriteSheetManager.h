#pragma once
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Types.h"
#include "tmxlite\Tileset.hpp"

#include "GMap.h"
#include "DynamicList.h"

class GString;
class RenderEngine;
class SpriteSheet;


// this thing should manage all of the information that the map is loading regarding sprites
class SpriteSheetManager
{
public:
	SpriteSheetManager();
	~SpriteSheetManager();
	
	AnimatedSprite getAnimatedSprite(const GString& file_path);

	void setRenderEngine(RenderEngine* render_engine) { _render_engine = render_engine; }

	// TODO: Rename this..
	void loadBitMapsForTileMap(const std::vector<tmx::Tileset>& tilesets);

	// ID from map goes in, sprite thing generated and comes out (returns null sprite if something goes wrong)
	Sprite getSpriteFromID(int id);
	DynamicList<RECTF_TYPE> getHitBoxesFromID(int id);

	// an animated file is going to need a json file to describe th animation..
	void loadAnimatedSprite(const GString& img_file, const GString& json_file);



private:
	RECTF_TYPE getRectF(const tmx::FloatRect& rect);

	GMap<int, DynamicList<RECTF_TYPE>> _tile_collision_walls;
	DynamicList<SpriteSheet*> _sprite_sheets;
	RenderEngine* _render_engine;

	GMap<UINT32, AnimatedSprite> _animated_sprites;
};






