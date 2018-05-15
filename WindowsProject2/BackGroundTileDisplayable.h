#pragma once
#include "Displayable.h"
#include "Sprite.h"


class BackGroundTileDisplayable :
	public DisplayableBitmap
{
public:
	BackGroundTileDisplayable();
	BackGroundTileDisplayable(float x, float y, float width, float height);
	~BackGroundTileDisplayable();

	Displayable::DrawType      getDrawType() const;
	//const Sprite& getSprite() const;
	//void          setSprite(const Sprite& sprite);

private:
	//Sprite _sprite;
};

