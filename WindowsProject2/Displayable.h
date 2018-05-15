#pragma once

#include "GameObject.h"
#include "Gstring.h"
#include "Sprite.h"


class Displayable : 
	public GameObject
{
public:

	enum DrawType 
	{
		Rectangle,
		Bitmap,
		Text,
		DontRender,
		AnimatedBitmap // might not be needed
	};

	Displayable();
	Displayable(float x, float y, float width, float height);
	virtual ~Displayable();

	virtual DrawType      getDrawType() const = 0;
};

class DisplayableBitmap :
	public Displayable
{
public:
	DisplayableBitmap() {}
	DisplayableBitmap(float x, float y, float width, float height) :
		Displayable(x, y, width, height)
	{
	}

	virtual DrawType      getDrawType() const { return Bitmap; }
	virtual const Sprite& getSprite() const { return _sprite; }
	virtual void          setSprite(const Sprite& sprite);

protected:
	Sprite _sprite;
};

class DisplayableText :
	public Displayable
{
public:
	DisplayableText() {}
	DisplayableText(const GString& text) : content(text) {}
	DisplayableText(float x, float y, float width, float height) :
		Displayable(x, y, width, height)
	{
	}

	virtual DrawType getDrawType() const { return Text; }
	const GString& getGString() { return content; }

protected:
	GString content;
};

class DisplayableAnimation :
	public Displayable
{
public:
	DisplayableAnimation() {}
	DisplayableAnimation(float x, float y, float width, float height) :
		Displayable(x, y, width, height)
	{
	}

	virtual DrawType      getDrawType() const { return Bitmap; }

	virtual const Sprite& getSprite() const = 0;
	virtual void          setSprite(const Sprite&) = 0;
};

