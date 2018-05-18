#include "BackGroundTileDisplayable.h"


BackGroundTileDisplayable::BackGroundTileDisplayable()
{
	_obj_type = "BackGroundTileDisplayable";
}

BackGroundTileDisplayable::BackGroundTileDisplayable(float x, float y, float width, float height) :
	DisplayableBitmap(x, y, width, height)
{
	_obj_type = "BackGroundTileDisplayable";
}

BackGroundTileDisplayable::~BackGroundTileDisplayable()
{
}

Displayable::DrawType BackGroundTileDisplayable::getDrawType() const
{
	return Displayable::Bitmap;
}

//const Sprite & BackGroundTileDisplayable::getSprite() const
//{
//	return _sprite;
//}
//
//void BackGroundTileDisplayable::setSprite(const Sprite& sprite)
//{
//	_sprite = sprite;
//	float width = _sprite.getSourceRect().right - _sprite.getSourceRect().left;
//	float height = _sprite.getSourceRect().bottom - _sprite.getSourceRect().top;
//	setSize(width, height);
//}


