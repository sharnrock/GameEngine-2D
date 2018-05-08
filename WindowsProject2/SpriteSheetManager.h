#pragma once
#include "Sprite.h"

// this thing should manage all of the information that the map is loading regarding sprites
class SpriteSheetManager
{
public:
	SpriteSheetManager();
	~SpriteSheetManager();

	// ID from map goes in, sprite thing generated and comes out
	Sprite getSpriteFromID() {}
};

