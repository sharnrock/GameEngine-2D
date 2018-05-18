#include "RenderBullshit.h"
#include "Displayable.h"
#include "GString.h"
#include "Sprite.h"
//#include "Types.h"
//#include <wincodec.h>

//#include "comptr.h"
#include <wrl/client.h>

// probably temporary
#include "Direct2DUtility.h"

// enable one of the following
#define USE_640_x_360
//#define USE_320_x_180

// Only game objects within the camera bounding rect will be displayed
//#define ONLY_DISPLAY_CAMERA_COLLISIONS

// Let the scaling be done while rendering, not while resizingf
//#define USE_SCALING 

// makes the camera use fine adjustement while moving
//#define SMOOTH_CAM_MOVE

//#include <Objbase.h>
//#include <comptr.h>

RenderBullshit::RenderBullshit() :
	m_hwnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL),
	/*m_pLightSlateGrayBrush(NULL),
	m_pCornflowerBlueBrush(NULL),*/
	m_pBlackBrush(NULL),
	_screen_ratio(Screen_16_9),
	_target_res_x(0),
	_target_res_y(0)
{
	window_size = D2D1::SizeU(200,200);
	// This is only temp!
	_displayables.append( DynamicList<Displayable*>(256) ); 
}

RenderBullshit::~RenderBullshit()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	//SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pBlackBrush);
	//SafeRelease(&m_pCornflowerBlueBrush);
}

void RenderBullshit::addDisplayableObject(Displayable* object, int layer)
{
	while (_displayables.count()-1 < layer)
	{
		_displayables.append( DynamicList<Displayable*>() );
	}
	_displayables[layer].append(object);
}

void RenderBullshit::clearDisplayables()
{
	_displayables.clear();
}

HRESULT RenderBullshit::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	// NEEDED FOR TEXT OUTPUT ON SCREEN
	static const WCHAR msc_fontName[] = L"Verdana";
	static const FLOAT msc_fontSize = 12;
	if (SUCCEEDED(hr))
	{

		// Create a DirectWrite factory.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_pDWriteFactory),
			reinterpret_cast<IUnknown **>(&m_pDWriteFactory)
		);
	}
	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = m_pDWriteFactory->CreateTextFormat(
			msc_fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			msc_fontSize,
			L"", //locale
			&m_pTextFormat
		);
	}
	// NEEDED FOR TEXT OUTPUT ON SCREEN

	return hr;
}



//Because this method will be called repeatedly, add an if statement to check whether the render target(m_pRenderTarget) already exists.The following code shows the complete CreateDeviceResources method.
HRESULT RenderBullshit::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);


		//if (SUCCEEDED(hr))
		//{
		//	// Create a gray brush.
		//	hr = m_pRenderTarget->CreateSolidColorBrush(
		//		D2D1::ColorF(D2D1::ColorF::LightSlateGray),
		//		&m_pLightSlateGrayBrush
		//	);
		//}
		//if (SUCCEEDED(hr))
		//{
		//	// Create a blue brush.
		//	hr = m_pRenderTarget->CreateSolidColorBrush(
		//		D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
		//		&m_pCornflowerBlueBrush
		//	);
		//}
		if (SUCCEEDED(hr))
		{
			// Create a blue brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black),
				&m_pBlackBrush
			);
		}
	}

	return hr;
}


//Implement the DemoApp::DiscardDeviceResources method.In this method, release the render target and the two brushes you created in the DemoApp::CreateDeviceResources method.
void RenderBullshit::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	//SafeRelease(&m_pLightSlateGrayBrush);
	//SafeRelease(&m_pCornflowerBlueBrush);
}


D2D1_SIZE_U RenderBullshit::getTargetResolution() const
{
	return D2D1::SizeU(_target_res_x, _target_res_y);
}

//Implement the DemoApp::OnRender method.First, create an HRESULT.Then call the CreateDeviceResource method.This method is called every time the window is painted.Recall that, in step 4 of Part 3, you added an if statement to prevent the method from doing any work if the render target already exists.

HRESULT RenderBullshit::OnRender()
{
	HRESULT hr = S_OK;
	hr = CreateDeviceResources();

	//Verify that the CreateDeviceResource method succeeded.If it didn't, don't perform any drawing.
	if (SUCCEEDED(hr))
	{
		//Inside the if statement you just created, initiate drawing by calling the render target's BeginDraw method. 
		//Set the render target's transform to the identity matrix, and clear the window.
		m_pRenderTarget->BeginDraw();
#ifndef USE_SCALING
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
#else
		// These only work on 16:9 ratios
#ifdef USE_640_x_360
		const float fake_width  = 640.0f;
		const float fake_height = 360.0f; 
#endif
#ifdef USE_320_x_180
		const float fake_width  = 320.0f;
		const float fake_height = 240.0f;
#endif

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(window_size.width/fake_width, window_size.height/fake_height));
#endif

		//m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		//Retrieve the size of the drawing area.
		//D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
	

		// Draw a rectangle for all the game objects
		for (int layer = 0; layer < _displayables.count(); layer++)
		{
			for (int i = 0; i < _displayables[layer].count(); i++)
			{
				if (!_displayables[layer].operator[](i)->isActive())
					continue;

				switch (_displayables[layer].operator[](i)->getDrawType())
				{
				case Displayable::Rectangle:
				{
					//m_pRenderTarget->FillRectangle(&_displayables[i]->getBoundingRect(), m_pCornflowerBlueBrush);
				}
				break;
				case Displayable::Bitmap:
				{
#ifdef ONLY_DISPLAY_CAMERA_COLLISIONS
					if (!_camera_view.hasCollisionWith(*_displayables[i]))
						break;
#endif
					DisplayableBitmap* displayable = static_cast<DisplayableBitmap*>(_displayables[layer][i]);
					const Sprite& sprite = displayable->getSprite();
					m_pRenderTarget->DrawBitmap(
						_bit_maps[sprite.getBitmapHandle()],
						offsetBoundingBoxWithCameraView(displayable->getBoundingRect()),
						1.0,
						D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,//D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, //
						sprite.getSourceRect()
					);
				}
				break;
				case Displayable::Text:
				{
					DisplayableText* text = static_cast<DisplayableText*>(_displayables[layer][i]);

					m_pRenderTarget->DrawText(
						text->getGString().toWideString().c_str(),
						text->getGString().getStringSize() - 1,
						m_pTextFormat,
						text->getBoundingRect(),
						m_pBlackBrush
						// draw text options,
						// dwrite measuring mode
					);
				}
				break;
				}
			}
		}







		//Call the render target's EndDraw method. The EndDraw method returns an HRESULT to indicate whether the drawing operations were successful. Close the if statement you began in Step 3.
		hr = m_pRenderTarget->EndDraw();
	}

	//Check the HRESULT returned by EndDraw.If it indicates that the render target needs to be recreated, call the DemoApp::DiscardDeviceResources method to release it; 
	//it will be recreated the next time the window receives a WM_PAINT or WM_DISPLAYCHANGE message.
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	//Return the HRESULT and close the method.
	return hr;
}


D2D1_RECT_F RenderBullshit::offsetBoundingBoxWithCameraView(const D2D1_RECT_F& world_coords)
{
	D2D1_RECT_F result;
#ifndef SMOOTH_CAM_MOVE
	result.left   = floor(world_coords.left   - _camera_view.X());
	result.right  = floor(world_coords.right  - _camera_view.X());
	result.top    = floor(world_coords.top    - _camera_view.Y());
	result.bottom = floor(world_coords.bottom - _camera_view.Y());
#else
	result.left   = world_coords.left   - _camera_view.X();
	result.right  = world_coords.right  - _camera_view.X();
	result.top    = world_coords.top    - _camera_view.Y();
	result.bottom = world_coords.bottom - _camera_view.Y();
#endif
	return result;
}


//Implement the DemoApp::OnResize method so that it resizes the render target to the new size of the window.

void RenderBullshit::OnResize(UINT width, UINT height)
{
	if (!m_pRenderTarget)
		return;
	
#ifdef USE_640_x_360
	const unsigned int screen_size = 40;
#endif
#ifdef USE_320_x_180
	const unsigned int screen_size = 20;
#endif
	
	unsigned int ratio_x;
	unsigned int ratio_y;
	
	switch (_screen_ratio)
	{
	case Screen_16_10:
	{
		ratio_x = 16;
		ratio_y = 10;
		break;
	}

	case Screen_4_3:
	{
		ratio_x = 4;
		ratio_y = 3;
		break;
	}
	case Screen_16_9:
	default:
	{
		ratio_x = 16;
		ratio_y = 9;
		break;
	}
	} // switch

#if 1
	_camera_view.setSize(
		(float)ratio_x*screen_size,
		(float)ratio_y*screen_size);
	_target_res_x = ratio_x * screen_size;
	_target_res_y = ratio_y * screen_size;
#else
	_camera_view.setSize(
		(float)width,
		(float)height);
#endif

	window_size.width = width;
	window_size.height = height;

#ifndef USE_SCALING
	m_pRenderTarget->Resize(D2D1::SizeU(
		ratio_x*screen_size, 
		ratio_y*screen_size
	));
#else
	m_pRenderTarget->Resize(D2D1::SizeU(
		width, height
	));
#endif



}

BITMAP_HANDL RenderBullshit::loadBitmapAssetFromFilepath(const GString& file_path, int width, int height)
{
	if (_bit_maps.contains(file_path.toHash()))
		return file_path.toHash();

	_bit_maps[file_path.toHash()] = nullptr;
	
	HRESULT result;
	result = Hilo::Direct2DHelpers::Direct2DUtility::LoadBitmapFromFile(
		m_pRenderTarget,
		GString(file_path).toWideString().c_str(),
		width,
		height,
		&_bit_maps[file_path.toHash()]
	);

	return file_path.toHash();
}






