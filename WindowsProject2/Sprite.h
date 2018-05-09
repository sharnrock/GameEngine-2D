#pragma once

#include <d2d1.h>

class Sprite
{
public:
	Sprite() {}
	Sprite(int bm_handle, int left, int top, int right, int bottom);
	~Sprite();

	int getBitmapHandle() const { return _bm_hndl; }
	D2D1_RECT_F getSourceRect() const;

	//Sprite& operator=(const Sprite& other);

private:
	int _bm_hndl;
	int left, top, right, bottom;
};

