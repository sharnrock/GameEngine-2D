#include "WindowsBullshit.h"
#include "HPElapsedTimer.h"
#include "GameLoop.h"
#include "RenderBullshit.h"
#include "HumanInputDeviceState.h"
#include "LevelLoader.h"
#include <Strsafe.h>
#include <Windowsx.h> // does this make windows.h obsolete?

WindowsBullshit::WindowsBullshit(GameLoop* game_loop, RenderBullshit* renderer, HumanInputDeviceState* hid_state) :
	game_loop(game_loop),
	render_bs(renderer),
	hid_state(hid_state),
	_level_loader(nullptr)
{
}

WindowsBullshit::~WindowsBullshit()
{
}

int WindowsBullshit::exec()
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		if (SUCCEEDED(initialize()))
			mainLoop();
		CoUninitialize(); 
	}
	return 0;
}

//#define LIMIT_FPS
void WindowsBullshit::mainLoop()
{
	MSG            msg;
	HPElapsedTimer timer;
	timer.start();
#ifdef LIMIT_FPS
	HPElapsedTimer fps_timer;
	fps_timer.start();
#endif
	msg.message = WM_NULL;

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
#ifdef LIMIT_FPS
			long long fps = 100;
			long long time_needed = 1E6 / fps; // time needed to hit specific fps 33,333 for 30fps
			long long elapsed = fps_timer.uElapsed(); // restart the timer
#endif

			this->game_loop->update(timer.uElapsed());
			this->render_bs->OnRender(); // should this be in game loop?
#ifdef LIMIT_FPS
			while (elapsed < time_needed)
			{
				elapsed += fps_timer.uElapsed();
			}
#endif
		}
	} 




}

HRESULT WindowsBullshit::initialize()
{
	HRESULT hr;

	// Initialize device-indpendent resources, such
	// as the Direct2D factory.
	hr = render_bs->CreateDeviceIndependentResources();
	

	if (SUCCEEDED(hr))
	{
		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WindowsBullshit::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"D2DDemoApp";

		RegisterClassEx(&wcex);


		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also the value it will use
		// to create its own windows.
		//render_bs->m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);
		render_bs->getDirect2DFactory()->GetDesktopDpi(&dpiX, &dpiY);

		// Create the window.
		render_bs->setHWND( CreateWindow(
			L"D2DDemoApp",
			L"Direct2D Demo App",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
			static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		));
		hr = render_bs->getHWND() ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(render_bs->getHWND(), SW_SHOWNORMAL);
			UpdateWindow(render_bs->getHWND());
		}

		if (_level_loader)
			_level_loader->loadLevel(); // now that coinitialize has been called, you're good
	}

	return hr;
}



LRESULT CALLBACK WindowsBullshit::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		WindowsBullshit *pDemoApp = (WindowsBullshit *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			PtrToUlong(pDemoApp)
		);

		result = 1;
	}
	else
	{
		WindowsBullshit *pDemoApp = reinterpret_cast<WindowsBullshit *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));

		bool wasHandled = false;

		if (pDemoApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->render_bs->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				pDemoApp->render_bs->OnRender();
				ValidateRect(hwnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;

			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				(*pDemoApp->hid_state)[(int)wParam] = (int)WM_KEYUP;
				result = 0;
			}
			break;
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				(*pDemoApp->hid_state)[(int)wParam] = (int)WM_KEYDOWN;
				result = 0;
			}
			break;
			case WM_MBUTTONUP:
			{
				(*pDemoApp->hid_state)[(int)wParam] = (int)WM_KEYUP;
				result = 0;
			}
			break;

			case WM_MBUTTONDOWN:
			{
				(*pDemoApp->hid_state)[(int)wParam] = (int)WM_KEYDOWN;
				result = 0;
			}
			break;
			case WM_MOUSEMOVE:
			{
				// TODO: probabbly the best place to do resolution math for mouse position
				D2D1_SIZE_U size = pDemoApp->render_bs->getTargetResolution();
				D2D1_SIZE_U realres = pDemoApp->render_bs->getWindowSize();

				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);

				xPos = xPos * size.width / realres.width;
				yPos = yPos * size.height / realres.height;

				pDemoApp->hid_state->setMouseCoords(xPos, yPos);
				result = 0;
			}
			break;



			} // switch
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}




