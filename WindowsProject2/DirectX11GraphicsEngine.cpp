#include "DirectX11GraphicsEngine.h"
#include "Displayable.h"
#include "GString.h"
#include "Sprite.h"
#include "ObjectFactory.h"
// probably temporary
#include "Direct2DUtility.h"




// enable one of the following
#define USE_640_x_360
//#define USE_320_x_180

// Only game objects within the camera bounding rect will be displayed
//#define ONLY_DISPLAY_CAMERA_COLLISIONS // TEMP: turned off while moving everything over to box2d

// Let the scaling be done while rendering, not while resizingf
//#define USE_SCALING 

// makes the camera use fine adjustement while moving
//#define SMOOTH_CAM_MOVE

// Draw the hit boxes of game objects
#define RENDER_HIT_BOXES

DirectX11GraphicsEngine::DirectX11GraphicsEngine() :
	_hwnd(NULL),
	_direct2d_factory(NULL),
	_render_target(NULL),
	_black_brush(NULL),
	_screen_ratio(Screen_16_9),
	_target_res_x(0),
	_target_res_y(0)
{
	_camera_view = static_cast<Camera*>(ObjectFactory::Instance().createCamera());
	_window_size = D2D1::SizeU(200,200);
	_displayables.append( DynamicList<Displayable*>(256) ); 
}

DirectX11GraphicsEngine::~DirectX11GraphicsEngine()
{
}

void DirectX11GraphicsEngine::addDisplayableObject(Displayable* object, int layer)
{
	while (_displayables.count()-1 < layer)
	{
		_displayables.append( DynamicList<Displayable*>() );
	}
	_displayables[layer].append(object);
}

void DirectX11GraphicsEngine::clearDisplayables()
{
	_displayables.clear();
}

HRESULT DirectX11GraphicsEngine::CreateDeviceResources()
{
	HRESULT hr = S_OK;
	//Because this method will be called repeatedly, add an if statement to check whether the render target(m_pRenderTarget) already exists.The following code shows the complete CreateDeviceResources method.

	if (!_render_target)
	{
		RECT rc;
		GetClientRect(_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		hr = _direct2d_factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(_hwnd, size),
			&_render_target
		);
		if (SUCCEEDED(hr))
		{
			// Create a brush for text display.
			hr = _render_target->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black),
				&_black_brush
			);
		}
	}

	return hr;
}

void DirectX11GraphicsEngine::DiscardDeviceResources()
{
	//Implement the DemoApp::DiscardDeviceResources method.In this method, release the render target and the two brushes you created in the DemoApp::CreateDeviceResources method.
	SafeRelease(&_render_target);
	SafeRelease(&_black_brush);
}

D2D1_SIZE_U DirectX11GraphicsEngine::getTargetResolution() const
{
	return D2D1::SizeU(_target_res_x, _target_res_y);
}

HRESULT DirectX11GraphicsEngine::initialize()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_direct2d_factory);

	// NEEDED FOR TEXT OUTPUT ON SCREEN
	// TODO: this might be able to be written better
	static const WCHAR msc_fontName[] = L"Verdana";
	static const FLOAT msc_fontSize = 12;

	if (SUCCEEDED(hr))
	{

		// Create a DirectWrite factory.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(_dwrite_factory),
			reinterpret_cast<IUnknown **>(&_dwrite_factory)
		);
	}
	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = _dwrite_factory->CreateTextFormat(
			msc_fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			msc_fontSize,
			L"", //locale
			&_text_format
		);
	}

	return hr;
}

void DirectX11GraphicsEngine::getDesktopDpi(float *dpiX, float *dpiY)
{
#pragma warning(suppress : 4996)
	this->getDirect2DFactory()->GetDesktopDpi(dpiX, dpiY);
}

HRESULT DirectX11GraphicsEngine::OnRender()
{
	HRESULT hr = S_OK;
	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		_render_target->BeginDraw();
#ifndef USE_SCALING
		_render_target->SetTransform(D2D1::Matrix3x2F::Identity());
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

		// Draw a rectangle for all the game objects
		for (int layer = 0; layer < _displayables.count(); layer++)
		{
			for (int i = 0; i < _displayables[layer].count(); i++)
			{
				if (!_displayables[layer].operator[](i)->isActive())
					continue;

				switch (_displayables[layer].operator[](i)->getDrawType())
				{

				case Displayable::Bitmap:
				{
#ifdef ONLY_DISPLAY_CAMERA_COLLISIONS
					if (!_camera_view->hasCoarseCollisionWith(_displayables[layer][i]))
						break;
#endif
					DisplayableBitmap* displayable = static_cast<DisplayableBitmap*>(_displayables[layer][i]);

					const Sprite& sprite = displayable->getSprite();
					_render_target->DrawBitmap(
						_bit_maps[sprite.getBitmapHandle()],
						offsetBoundingBoxWithCameraView(displayable->getBoundingRect()),
						1.0,
						D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,//D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, //
						sprite.getSourceRect()
					);

					
#ifdef RENDER_HIT_BOXES // This should definitely be in another part of the code.. Move it to game loop or something so the argumetn acn be passed from cmdline
					for (int i = 0; i < displayable->getFineCollisionBoxes().count(); i++)
					{
						VECTORF cpos = displayable->getWorldCenterCoords();
						RECTF_TYPE hbox = displayable->getFineCollisionBoxes().at(i);
						float half_w = (hbox.right - hbox.left) *0.5f;
						float half_h = (hbox.bottom - hbox.top) *0.5f;
						RECTF_TYPE result;
						result.left   = cpos.x - half_w;
						result.right  = cpos.x + half_w;
						result.top    = cpos.y - half_h;
						result.bottom = cpos.y + half_h;
						auto a = offsetBoundingBoxWithCameraView(result);
						_render_target->DrawRectangle(&a, _black_brush);
					}
#endif
				}
				break;
				case Displayable::Rectangle:
				{
					auto a = offsetBoundingBoxWithCameraView(_displayables[layer][i]->getBoundingRect());
					_render_target->FillRectangle(&a, _black_brush);
				}
				break;
				case Displayable::Text:
				{
					DisplayableText* text = static_cast<DisplayableText*>(_displayables[layer][i]);

					_render_target->DrawText(
						text->getGString().toWideString().c_str(),
						text->getGString().getStringSize() - 1,
						_text_format,
						text->getBoundingRect(),
						_black_brush
						// draw text options,
						// dwrite measuring mode
					);
				}
				break;
				}
			}
		}

		hr = _render_target->EndDraw();
	}

	//Check the HRESULT returned by EndDraw.If it indicates that the render target needs to be recreated, call the DemoApp::DiscardDeviceResources method to release it; 
	//it will be recreated the next time the window receives a WM_PAINT or WM_DISPLAYCHANGE message.
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	
	return hr;
}


D2D1_RECT_F DirectX11GraphicsEngine::offsetBoundingBoxWithCameraView(const D2D1_RECT_F& world_coords)
{
	D2D1_RECT_F result;

	result.left   = floor(world_coords.left   - _camera_view->X());
	result.right  = floor(world_coords.right  - _camera_view->X());
	result.top    = floor(world_coords.top    - _camera_view->Y());
	result.bottom = floor(world_coords.bottom - _camera_view->Y());

	return result;
}

HRESULT DirectX11GraphicsEngine::uninitialize()
{
	DiscardDeviceResources();

	SafeRelease(&_dwrite_factory);
	SafeRelease(&_direct2d_factory);
	SafeRelease(&_text_format);

	return S_OK;
}

void DirectX11GraphicsEngine::OnResize(UINT width, UINT height)
{
	if (!_render_target)
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
	_camera_view->setSize(
		(float)ratio_x*screen_size,
		(float)ratio_y*screen_size);
	_target_res_x = ratio_x * screen_size;
	_target_res_y = ratio_y * screen_size;
#else
	_camera_view->setSize(
		(float)width,
		(float)height);
#endif

	_window_size.width = width;
	_window_size.height = height;

#ifndef USE_SCALING
	_render_target->Resize(D2D1::SizeU(
		ratio_x*screen_size, 
		ratio_y*screen_size
	));
#else
	m_pRenderTarget->Resize(D2D1::SizeU(
		width, height
	));
#endif

}

BITMAP_HANDL DirectX11GraphicsEngine::loadBitmapAssetFromFilepath(const GString& file_path, int width, int height)
{
	if (_bit_maps.contains(file_path.toHash()))
		return file_path.toHash();

	_bit_maps[file_path.toHash()] = nullptr;
	
	HRESULT result;
	result = Hilo::Direct2DHelpers::Direct2DUtility::LoadBitmapFromFile(
		_render_target,
		GString(file_path).toWideString().c_str(),
		width,
		height,
		&_bit_maps[file_path.toHash()]
	);

	return file_path.toHash();
}

