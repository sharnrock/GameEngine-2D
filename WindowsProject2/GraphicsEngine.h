#pragma once

#include "Types.h"

class Camera;
class Displayable;
class GString;

// TODO: clean all these functions up
class GraphicsEngine
{
public:
	virtual ~GraphicsEngine();

	enum ScreenRatio
	{
		Screen_16_9,
		Screen_16_10,
		Screen_4_3,
	};

	virtual HRESULT initialize() = 0;
	virtual HRESULT uninitialize() = 0;

	// stolen from windows
	virtual void getDesktopDpi(float *dpiX, float *dpiY) = 0;

	virtual void setHWND(HWND hwnd) = 0;

	virtual void addDisplayableObject(Displayable* object, int layer = 0) = 0;

	virtual D2D1_SIZE_U getTargetResolution() const = 0;
	virtual D2D1_SIZE_U getWindowSize() const = 0;
	virtual void    OnResize(UINT width, UINT height) = 0;
	virtual HRESULT OnRender() = 0;

	virtual void         setScreenRatio(ScreenRatio ratio) = 0;
	virtual ScreenRatio  getScreenRatio() const = 0;

	//virtual void         addDisplayableObject(Displayable* object, int layer) = 0;
	virtual void         clearDisplayables() = 0;
	
	virtual BITMAP_HANDL loadBitmapAssetFromFilepath(const GString& file_path, int width, int height) = 0;

	virtual Camera*      getCamera() = 0;
};

