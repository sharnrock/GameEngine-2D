#include "Sprite.h"



Sprite::Sprite(int bm_handle, int left, int top, int right, int bottom) :
	_is_null(false),
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
	RECTF_TYPE result;

	result.left   = (float)left;
	result.right  = (float)right;
	result.top    = (float)top;
	result.bottom = (float)bottom;

	return result;
}

float Sprite::getWidth() const
{
	return (float)(right - left);
}

float Sprite::getHeight() const
{
	return (float)(bottom - top);
}