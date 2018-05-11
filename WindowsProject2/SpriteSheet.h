#pragma once

#include "Types.h"
#include "Sprite.h"
#include <memory>

class SpriteSheet
{
public:
	SpriteSheet(int image_handle, int tile_width, int tile_height, int col_count);
	~SpriteSheet();

	void setBitmapHandle(BITMAP_HANDL hndl) { _bm_hndl = hndl; }
	BITMAP_HANDL  getBitmapHandle() const { return _bm_hndl; }

	// example 32x32 sprite
	void setSpriteSize(int width, int height); 
	int getSpriteHeight() const { return _sprite_height; }
	int getSpriteWidth() const { return _sprite_width; }

	// This might not be necessary with new sprite manager
	void setFirstGid(int first_gid) { _first_gid = first_gid; }
	int getFirstGid() const { return _first_gid;  }

	// last gid
	void setLastGid(int last_gid) { _last_gid = last_gid; }
	int getLastGid() const { return _last_gid; }


	int maxColumns() const { return _max_columns; }

	// maybe not
	Sprite createSprite(int column, int row);

protected:
	int _bm_hndl;
	int _sprite_height;
	int _sprite_width;
	int _max_columns;
	int _first_gid;
	int _last_gid;

};

