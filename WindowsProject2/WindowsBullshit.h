#pragma once


// TODO: move as many of these as possible out of the header file
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

class GameLoop;
class RenderBullshit;
class HumanInputDeviceState;
class LevelLoader; 

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif



#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif



class WindowsBullshit
{
public:
	WindowsBullshit(GameLoop* game_loop, RenderBullshit* renderer, HumanInputDeviceState* hid_state);
	~WindowsBullshit();

	int exec();

	LevelLoader * _level_loader;


private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT                 initialize();
	void                    mainLoop();

	GameLoop*               game_loop;
	RenderBullshit*         render_bs;
	HumanInputDeviceState*  hid_state;

};

