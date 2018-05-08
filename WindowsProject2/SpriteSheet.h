#pragma once

#include "Sprite.h"
#include <memory>

class SpriteSheet
{
public:
	SpriteSheet(int image_handle, int tile_width, int tile_height, int col_count);
	~SpriteSheet();

	void setBitmapHandle(int hndl) { _bm_hndl = hndl; }
	int  getBitmapHandle() const { return _bm_hndl; }

	// example 32x32 sprite
	void setSpriteSize(int width, int height); 
	int getSpriteHeight() const { return _sprite_height; }
	int getSpriteWidth() const { return _sprite_width; }

	
	int maxColumns() const { return _max_columns; }
	//int maxRows() const { return _max_rows; }

	// maybe not
	Sprite createSprite(int column, int row);

protected:
	int _bm_hndl;
	int _sprite_height;
	int _sprite_width;
	int _max_columns;
	int _max_rows;
};

