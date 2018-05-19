#pragma once

#include "Types.h"

class Sprite
{
public:

	enum Transform
	{
		None           = 0x00,
		FlipHorizontal = 0x01,
		FlipVertical   = 0x02,
		Rotate         = 0x04,
		Skew           = 0x08
	};

	Sprite();
	Sprite(int bm_handle, int left, int top, int right, int bottom);
	~Sprite();

	int getBitmapHandle() const { return _bm_hndl; }
	RECTF_TYPE getSourceRect() const;

	bool isNull() const { return _is_null; }

	float getWidth() const;
	float getHeight() const;

	// also sets the rotate flag on transform
	void  setRotation(float rads);
	float getRotation() const { return _rotation; }

	// Can set multiple transforms with bitwise '|'
	void setTransform(UINT32 t) { _transform = t; }
	UINT32 getTransform() const { return _transform; }

private:
	bool _is_null;
	int _bm_hndl;
	int left, top, right, bottom;
	UINT32 _transform;
	float _rotation;
};

