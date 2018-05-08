#pragma once

class Camera;
class Displayable;

// TODO: sort out this forward declare
#include <string>
//namespace std {	class string; }


class RenderEngine
{
public:
	virtual ~RenderEngine();

	enum ScreenRatio
	{
		Screen_16_9,
		Screen_16_10,
		Screen_4_3,
	};

	virtual void        setScreenRatio(ScreenRatio ratio) = 0;
	virtual ScreenRatio getScreenRatio() const = 0;

	virtual void        addDisplayableObject(Displayable* object, int layer) = 0;
	virtual void        clearDisplayables() = 0;
	
	virtual int         loadBitmapAssetFromFilepath(const std::string& file_path, int width, int height) = 0;

	virtual Camera*     getCamera() = 0;
};

