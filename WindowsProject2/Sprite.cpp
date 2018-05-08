#include "Sprite.h"



Sprite::Sprite(int bm_handle, int left, int top, int right, int bottom) :
	_bm_hndl(bm_handle),
	left(left), 
	top(top), 
	right(right), 
	bottom(bottom)
{
}

Sprite::~Sprite()
{
}

D2D1_RECT_F Sprite::getSourceRect() const
{
	return D2D1::RectF(
		(float)left, 
		(float)top, 
		(float)right, 
		(float)bottom
	); // portion of bitmap
}