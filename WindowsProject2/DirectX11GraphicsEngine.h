#pragma once

// TODO: clean this up ; the whole .h and .cpp
#include "GraphicsEngine.h"
#include "DynamicList.h"
#include "Camera.h" 
#include "GMap.h"
#include "Types.h"

class Displayable;
class GString;
struct ID2D1Factory;
struct ID2D1HwndRenderTarget;
struct ID2D1SolidColorBrush;
struct ID2D1Bitmap;
struct IDWriteFactory;
struct IDWriteTextFormat;


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
	HRESULT CreateDeviceResources();
	void    DiscardDeviceResources();

	HRESULT OnRender();
	void    OnResize(UINT width, UINT height);
	
	void        setScreenRatio(ScreenRatio ratio) { _screen_ratio = ratio; }
	ScreenRatio getScreenRatio() const { return _screen_ratio; }
	void        getDesktopDpi(float *dpiX, float *dpiY);
	D2D1_SIZE_U getWindowSize() const { return _window_size; }
	D2D1_SIZE_U getTargetResolution() const;

	void                   setHWND(HWND hwnd) { _hwnd = hwnd; }
	ID2D1Factory*          getDirect2DFactory() { return _direct2d_factory; }
	ID2D1HwndRenderTarget* getRenderTarget() { return _render_target; }

	// loads the bitmap into memory and returns a handle for it
	BITMAP_HANDL loadBitmapAssetFromFilepath(const GString& file_path, int width, int height);
	void         addDisplayableObject(Displayable* object, int layer = 0);
	void         clearDisplayables();
	Camera*      getCamera() { return _camera_view; }

private: 
	// converts world coordinates to screen coordinates
	// returns newly minted rect
	D2D1_RECT_F offsetBoundingBoxWithCameraView(const D2D1_RECT_F& world_coords);

	HWND                                    _hwnd;
	ID2D1Factory*                           _direct2d_factory;
	ID2D1HwndRenderTarget*                  _render_target;
	ID2D1SolidColorBrush*                   _black_brush;
	DynamicList<DynamicList<Displayable*> > _displayables;
	GMap<BITMAP_HANDL, ID2D1Bitmap*>        _bit_maps;
	IDWriteFactory*                         _dwrite_factory;
	IDWriteTextFormat*                      _text_format;
	Camera*                                 _camera_view;
	D2D1_SIZE_U                             _window_size;
	ScreenRatio                             _screen_ratio;

	UINT32                                  _target_res_x;
	UINT32                                  _target_res_y;
};

