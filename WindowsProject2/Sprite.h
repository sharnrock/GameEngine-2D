#pragma once

#include "Types.h"

class Sprite
{
public:

	// TODO: add section for transform matrix stuff can be transmitted like rotate or skew etc..

	Sprite() : _is_null(true) {}
	Sprite(int bm_handle, int left, int top, int right, int bottom);
	~Sprite();

	int getBitmapHandle() const { return _bm_hndl; }
	RECTF_TYPE getSourceRect() const;

	bool isNull() const { return _is_null; }

	float getWidth() const;
	float getHeight() const;

private:
	bool _is_null;
	int _bm_hndl;
	int left, top, right, bottom;
};

