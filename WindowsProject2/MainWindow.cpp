#include "MainWindow.h"
#include "HPElapsedTimer.h"
#include "MainLoop.h"
#include "AudioEngine.h"
#include "MainLoop.h"
#include "GraphicsEngine.h"

#include <Windowsx.h>
#include <assert.h>



MainWindow::MainWindow() :
	graphics(nullptr),
	audio(nullptr),
	main_loop(nullptr)
{
}

void MainWindow::setGraphicsSystem(GraphicsEngine* graphics) 
{ 
	this->graphics = graphics; 
}

void MainWindow::setAudioSystem(AudioEngine* audio) 
{ 
	this->audio = audio; 
}

void MainWindow::setMainLoop(MainLoop* main_loop) 
{ 
	this->main_loop = main_loop; 
}

void MainWindow::mainLoop()
{
	MSG            msg;
	msg.message = WM_NULL;

	HPElapsedTimer timer;
	timer.start();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			this->main_loop->update(timer.uElapsed());
		}
	}
}

HRESULT MainWindow::initialize()
{
	HRESULT hr = S_OK;

	assert(this->graphics);
	assert(this->audio);
	assert(this->main_loop);

	if (SUCCEEDED(hr = this->graphics->initialize()))
	if (SUCCEEDED(hr = this->audio->initialize()))
	if (SUCCEEDED(hr))
	{
		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = MainWindow::WndProc;
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
		this->graphics->getDesktopDpi(&dpiX, &dpiY);
		//render_bs->getDirect2DFactory()->GetDesktopDpi(&dpiX, &dpiY);

		// Create the window.
		this->hwnd = (CreateWindow(
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
		hr = this->hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			this->graphics->setHWND(this->hwnd);
			ShowWindow(this->hwnd, SW_SHOWNORMAL);
			UpdateWindow(this->hwnd);

			// this will load level
			hr = this->main_loop->initialize();
			this->graphics->OnResize(640, 480);
		}
	}

	return hr;
}

HRESULT MainWindow::uninitialize()
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = this->audio->uninitialize()))
		if (SUCCEEDED(hr = this->graphics->uninitialize()))
			hr = this->main_loop->uninitialize();

	return hr;
}

int MainWindow::exec()
{
	assert(this->graphics);
	assert(this->audio);
	assert(this->main_loop);

	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		if (SUCCEEDED(this->initialize()))
			this->mainLoop();

		if (SUCCEEDED(this->uninitialize()))
			CoUninitialize();

		return 0;
	}

	return 1;
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		MainWindow *_this = (MainWindow *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			(LONG_PTR)_this
		);

		result = 1;
	}
	else
	{
		LONG_PTR lpter = GetWindowLongPtrW(hwnd, GWLP_USERDATA);
		MainWindow* pDemoApp = (MainWindow*)lpter;

		bool wasHandled = false;

		if (pDemoApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->graphics->OnResize(width, height);
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
				pDemoApp->graphics->OnRender();
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
				pDemoApp->hid_state[(int)wParam] = (int)WM_KEYUP;
				result = 0;
			}
			break;
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				pDemoApp->hid_state[(int)wParam] = (int)WM_KEYDOWN;
				result = 0;
			}
			break;
			case WM_LBUTTONDOWN:
			{
				pDemoApp->hid_state[MK_LBUTTON] = (int)WM_KEYDOWN; 
				result = 0;
			}
			break;
			case WM_LBUTTONUP:
			{
				pDemoApp->hid_state[MK_LBUTTON] = (int)WM_KEYUP;
				result = 0;
			}
			break;
#if 0 // optional mouse keys
			WM_LBUTTONDOWN	The left mouse button was pressed.
				WM_LBUTTONUP	The left mouse button was released.
				WM_MBUTTONDBLCLK	The middle mouse button was double - clicked.
				WM_MBUTTONDOWN	The middle mouse button was pressed.
				WM_MBUTTONUP	The middle mouse button was released.
				WM_RBUTTONDBLCLK	The right mouse button was double - clicked.
				WM_RBUTTONDOWN	The right mouse button was pressed.
				WM_RBUTTONUP	The right mouse button was released.
				WM_XBUTTONDBLCLK	An X mouse button was double - clicked.
				WM_XBUTTONDOWN	An X mouse button was pressed.
				WM_XBUTTONUP	An X mouse button was released.
#endif




			case WM_MOUSEMOVE:
			{
				// TODO: probabbly the best place to do resolution math for mouse position
				D2D1_SIZE_U size = pDemoApp->graphics->getTargetResolution();
				D2D1_SIZE_U realres = pDemoApp->graphics->getWindowSize();

				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);

				xPos = xPos * size.width / realres.width;
				yPos = yPos * size.height / realres.height;

				pDemoApp->hid_state.setMouseCoords(xPos, yPos);
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














