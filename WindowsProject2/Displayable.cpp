#include "Displayable.h"



Displayable::Displayable()
{
}

Displayable::Displayable(float x, float y, float width, float height) :
	GameObject(x,y,width,height)
{

}


Displayable::~Displayable()
{
}

void DisplayableBitmap::setSprite(const Sprite& sprite) 
{
	_sprite = sprite;	
	float width = _sprite.getSourceRect().right - _sprite.getSourceRect().left;
	float height = _sprite.getSourceRect().bottom - _sprite.getSourceRect().top;
	setSize(width, height);
}