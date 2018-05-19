#pragma once

// TODO: clean this up ; the whole .h and .cpp
#include "DynamicList.h"
#include "GameObject.h"
class Displayable;
#include "GraphicsEngine.h"
#include "Camera.h"
#include "GMap.h"

#include "Types.h"
class GString;

#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>



#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

class DirectX11GraphicsEngine :
	public GraphicsEngine
{
public:

	DirectX11GraphicsEngine();
	~DirectX11GraphicsEngine();

	HRESULT initialize();
	HRESULT uninitialize();

	void getDesktopDpi(float *dpiX, float *dpiY);
	HRESULT OnRender();
	void    OnResize(UINT width, UINT height);
	
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	void    DiscardDeviceResources();
	
	void setScreenRatio(ScreenRatio ratio) { _screen_ratio = ratio; }
	ScreenRatio getScreenRatio() const { return _screen_ratio; }
	D2D1_SIZE_U getWindowSize() const { return window_size; }
	D2D1_SIZE_U getTargetResolution() const;




	HWND getHWND() { return m_hwnd; }
	ID2D1Factory* getDirect2DFactory() { return m_pDirect2dFactory; }
	ID2D1HwndRenderTarget* getRenderTarget() { return m_pRenderTarget; }

	void setHWND(HWND hwnd) { m_hwnd = hwnd; }
	void setD2DFactory(ID2D1Factory* factory) { m_pDirect2dFactory = factory; }

	void addDisplayableObject(Displayable* object, int layer = 0);
	void clearDisplayables();

	// loads the bitmap into memory and returns a handle for it
	// TODO: probably going to come up with a type instead of int; even it it's typedef int BMHANDLE
	BITMAP_HANDL loadBitmapAssetFromFilepath(const GString& file_path, int width, int height);

	Camera*      getCamera() { return &_camera_view; }
	
private: 
	// converts world coordinates to screen coordinates
	// returns newly minted rect
	D2D1_RECT_F offsetBoundingBoxWithCameraView(const D2D1_RECT_F& world_coords);

	HWND                                    m_hwnd;
	ID2D1Factory*                           m_pDirect2dFactory;
	ID2D1HwndRenderTarget*                  m_pRenderTarget;
	ID2D1SolidColorBrush*                   m_pBlackBrush;
	DynamicList<DynamicList<Displayable*> > _displayables;
	GMap<BITMAP_HANDL, ID2D1Bitmap*>        _bit_maps;
	Camera                                  _camera_view;
	D2D1_SIZE_U                             window_size;
	ScreenRatio                             _screen_ratio;
	IDWriteFactory*                         m_pDWriteFactory;
	IDWriteTextFormat*                      m_pTextFormat;

	UINT32                                  _target_res_x;
	UINT32                                  _target_res_y;


};

