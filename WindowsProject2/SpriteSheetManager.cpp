#include "SpriteSheetManager.h"
#include "SpriteSheet.h"
#include "RenderEngine.h"
#include "GString.h"
#include "document.h"

#include "tmxlite\Tileset.hpp"



SpriteSheetManager::SpriteSheetManager() :
	_render_engine(nullptr)
{
}

SpriteSheetManager::~SpriteSheetManager()
{
}

AnimatedSprite SpriteSheetManager::getAnimatedSprite(const GString& file_path)
{
	return _animated_sprites[file_path.toHash()];
}

void SpriteSheetManager::loadBitMapsForTileMap(const std::vector<tmx::Tileset>& tilesets)
{
	assert(_render_engine);

	if (tilesets.empty())
		return;

	for (int i = 0; i < tilesets.size(); i++)
	{

		// This will determine all the IDs that have collision
		const std::vector<tmx::Tileset::Tile>& tiles = tilesets[i].getTiles();
		for (int tile_props = 0; tile_props < tiles.size(); ++tile_props)
		{
			for (int rect = 0; rect < tiles[tile_props].objectGroup.getObjects().size(); rect++)
			{
				// coming out of tag:x="7.27273" y="6.90909" width="24.1818" height="24.7273"
				RECTF_TYPE new_rect = getRectF( tiles[tile_props].objectGroup.getObjects().at(rect).getAABB() );
				_tile_collision_walls[tiles[tile_props].ID + tilesets[i].getFirstGID()].append(new_rect);
			}
		}

		GString gstr(tilesets[i].getImagePath());
		OutputDebugString(gstr.toWideString().c_str());
		OutputDebugString(TEXT("\n"));

		int width = 0, height = 0;
		tilesets[i].getImageConstraints(width, height);
		BITMAP_HANDL image_handle = _render_engine->loadBitmapAssetFromFilepath(tilesets[i].getImagePath(), width, height);

		int tile_width = tilesets[i].getTileSize().x;
		int tile_height = tilesets[i].getTileSize().y;
		int max_cols = tilesets[i].getColumnCount();

		SpriteSheet* ss = new SpriteSheet(image_handle, tile_width, tile_height, max_cols);
		ss->setFirstGid(tilesets[i].getFirstGID());
		ss->setLastGid(tilesets[i].getLastGID());
		_sprite_sheets.append(ss);
	}
}

RECTF_TYPE SpriteSheetManager::getRectF(const tmx::FloatRect& rect)
{
	RECTF_TYPE new_rect;
	new_rect.left = rect.left;
	new_rect.right = rect.width + new_rect.left;
	new_rect.top = rect.top;
	new_rect.bottom = rect.height + new_rect.top;
	return new_rect;
}

Sprite SpriteSheetManager::getSpriteFromID(int id)
{
	if (_sprite_sheets.isEmpty())
		return Sprite();

	int first_gid = 0;
	int sheet_ndx = 0;
	int delta = _sprite_sheets.front()->getFirstGid();
	for (int i = 0; i < _sprite_sheets.count(); i++)
	{
		if (_sprite_sheets.at(i)->getFirstGid() <= id && id <= _sprite_sheets.at(i)->getLastGid())
		{
			first_gid = _sprite_sheets.at(i)->getFirstGid();
			sheet_ndx = i;
			break;
		}
	}

	int col = (id - first_gid) % _sprite_sheets[sheet_ndx]->maxColumns();
	int row = (id - first_gid) / _sprite_sheets[sheet_ndx]->maxColumns();
	return _sprite_sheets[sheet_ndx]->createSprite(col, row);
}

DynamicList<RECTF_TYPE> SpriteSheetManager::getHitBoxesFromID(int id)
{
	DynamicList<RECTF_TYPE> result = DynamicList<RECTF_TYPE>();
	if (!_tile_collision_walls.contains(id))
		return result;

	result = _tile_collision_walls[id];
	return result;
}




#include "GFile.h"

void SpriteSheetManager::loadAnimatedSprite(const GString& img_file, const GString& json_file)
{
	// TODO: this code can be cleaned up
	AnimatedSprite new_ani_sprite;
	assert(_render_engine);

	// read file into string
	GFile gjson_file(json_file);
	std::string json_str = gjson_file.getFileAsString();
	
	// put string into parser
	rapidjson::Document document;
	document.Parse(json_str.c_str());
	
	assert(document["frames"].IsArray());
	const rapidjson::Value& a = document["frames"];
	assert(a.IsArray());
	
	new_ani_sprite.setFrameCount(a.Size());
	new_ani_sprite.setFrameDuration(a[0]["duration"].GetInt());
	int sprite_w = a[0]["sourceSize"]["w"].GetInt();
	int sprite_h = a[0]["sourceSize"]["h"].GetInt();
	new_ani_sprite.setSpriteSize(sprite_w, sprite_h);

	assert(document["meta"].IsObject());
	const rapidjson::Value& meta = document["meta"];
	assert(meta["size"].IsObject());
	int width  = meta["size"]["w"].GetInt();
	int height = meta["size"]["h"].GetInt();

	BITMAP_HANDL hndl = _render_engine->loadBitmapAssetFromFilepath(img_file.toStdString(), width, height);
	new_ani_sprite.setBitmapHandle(hndl);
	_animated_sprites[img_file.toHash()] = new_ani_sprite;
}
