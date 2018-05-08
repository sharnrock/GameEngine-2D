//#include "DemoApp.h"
#include "WindowsBullshit.h"
#include "GameLoop.h"
#include "RenderBullshit.h"
#include "LevelLoader.h"
#include "HumanInputDeviceState.h"

int WINAPI WinMain(HINSTANCE /* hInstance */, HINSTANCE /* hPrevInstance */, LPSTR /* lpCmdLine */, int /* nCmdShow */)
{
	HumanInputDeviceState hid_state;
	GameLoop game_loop;
	RenderBullshit renderer;
	LevelLoader loader;
	loader.setGameLoop(&game_loop);
	loader.setRenderBullshit(&renderer);
	loader.setHumanInputDeviceState(&hid_state);
	WindowsBullshit app(&game_loop, &renderer, &hid_state);

	app._level_loader = &loader; // oh god, clean this up
	return app.exec();
}