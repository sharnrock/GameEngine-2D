#pragma once


#include "HumanInputDeviceState.h"
#include <windows.h>

class MainLoop;
class GraphicsEngine;
class AudioEngine;

#if 0 // We might be able to get rid of this...
#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif
#endif

// Module loading..
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif



class MainWindow
{
public:
	MainWindow();

	void setGraphicsSystem        (GraphicsEngine* graphics);
	void setAudioSystem           (AudioEngine* audio);
	void setMainLoop              (MainLoop* main_loop);

	int  exec();

	HWND getHwnd() { return hwnd; }
	const HumanInputDeviceState* getHumanInputDeviceState() const { return &this->hid_state; }

private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Updates the windows message loop and updates the main_loop update function
	void    mainLoop();

	// called after coinitialize: initializes all the sub systems
	// main_loop initialized after window is shown
	HRESULT initialize(); 

	// called before couninitialize: uninitializes all the sub systems
	HRESULT uninitialize();   

	GraphicsEngine*          graphics; 
	AudioEngine*           audio;
	MainLoop*              main_loop; 
	
	HumanInputDeviceState  hid_state;
	HWND                   hwnd;
};