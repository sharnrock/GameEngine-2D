#include "SpriteSheet.h"



SpriteSheet::SpriteSheet(int image_handle, int tile_width, int tile_height, int col_count) :
	_bm_hndl(image_handle),
	_sprite_width(tile_width),
	_sprite_height(tile_height),
	_max_columns(col_count),
	_first_gid(0),
	_last_gid(0)
{
}


SpriteSheet::~SpriteSheet()
{
}

void SpriteSheet::setSpriteSize(int width, int height) 
{ 
	_sprite_height = height; 
	_sprite_width = width; 
}

Sprite SpriteSheet::createSprite(int column, int row)
{ 
	return Sprite(
		_bm_hndl,
		_sprite_width*column, 
		_sprite_height*row,
		_sprite_width*(column+1),
		_sprite_height*(row+1)
	); 
}