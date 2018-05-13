#pragma once

// TODO: clean this up
#include "DynamicList.h"
#include "GameObject.h"
class Displayable;
#include "RenderEngine.h"
#include "Camera.h"

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

#include <string>

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

class RenderBullshit :
	public RenderEngine
{
public:
	//enum ScreenRatio
	//{
	//	Screen_16_9,
	//	Screen_16_10,
	//	Screen_4_3,
	//};

	RenderBullshit();
	~RenderBullshit();
	//void releaseAll();

	HRESULT OnRender();
	void    OnResize(UINT width, UINT height);
	
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	void    DiscardDeviceResources();
	
	void setScreenRatio(ScreenRatio ratio) { _screen_ratio = ratio; }
	ScreenRatio getScreenRatio() const { return _screen_ratio; }
	D2D1_SIZE_U getWindowSize() const { return window_size; }

	HWND getHWND() { return m_hwnd; }
	ID2D1Factory* getDirect2DFactory() { return m_pDirect2dFactory; }
	ID2D1HwndRenderTarget* getRenderTarget() { return m_pRenderTarget; }
	ID2D1SolidColorBrush* getGrayBrush() const { return m_pLightSlateGrayBrush; }
	ID2D1SolidColorBrush* getBlueBrush() const { return m_pCornflowerBlueBrush; }
	ID2D1SolidColorBrush* getBlackBrush() const { return m_pBlackBrush; }

	void setHWND(HWND hwnd) { m_hwnd = hwnd; }
	void setD2DFactory(ID2D1Factory* factory) { m_pDirect2dFactory = factory; }
	void setGrayBrush(ID2D1SolidColorBrush* brush) { m_pLightSlateGrayBrush = brush; }
	void setBlueBrush(ID2D1SolidColorBrush* brush) { m_pCornflowerBlueBrush = brush; }
	void setBlackBrush(ID2D1SolidColorBrush* brush) { m_pBlackBrush = brush; }

	void addDisplayableObject(Displayable* object, int layer = 0);
	void clearDisplayables();

	// loads the bitmap into memory and returns a handle for it
	// TODO: probably going to come up with a type instead of int; even it it's typedef int BMHANDLE
	int loadBitmapAssetFromFilepath(const std::string& file_path, int width, int height);

	Camera* getCamera() { return &_camera_view; }
	
private: 
	// converts world coordinates to screen coordinates
	// returns newly minted rect
	D2D1_RECT_F offsetBoundingBoxWithCameraView(const D2D1_RECT_F& world_coords);

	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
	ID2D1SolidColorBrush* m_pBlackBrush;
	DynamicList<DynamicList<Displayable*> > _displayables;
	//DynamicList<Displayable*> _displayables;
	DynamicList<ID2D1Bitmap*> _bit_maps;
	Camera _camera_view;
	D2D1_SIZE_U window_size;
	ScreenRatio _screen_ratio;
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
};

