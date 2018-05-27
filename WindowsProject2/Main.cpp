#include "MainWindow.h"
#include "DirectX11GraphicsEngine.h"
#include "GameLoop.h"
#include "LevelLoader.h"
#include "AudioEngineFactory.h"
#include "Box2dPhysicsEngine.h"
#include "ObjectFactory.h"

int WINAPI WinMain(HINSTANCE /* hInstance */, HINSTANCE /* hPrevInstance */, LPSTR /* lpCmdLine */, int /* nCmdShow */)
{
	GameLoop                game_loop;
	DirectX11GraphicsEngine renderer;
	LevelLoader             loader;
	AudioEngine*            audio = AudioEngineFactory::getAudioEngine();
	Box2dPhysicsEngine      physics;
	MainWindow              w;
	
	ObjectFactory::Instance().initialize(&game_loop, &renderer, loader.getSpriteManager(), audio, &physics);

	// Set up level loader
	loader.setGameLoop(&game_loop);
	loader.setRenderBullshit(&renderer);
	loader.setHumanInputDeviceState(w.getHumanInputDeviceState());

	// Set up game loop
	game_loop.setLevelLoader(&loader);
	game_loop.setGraphicsEngine(&renderer);
	game_loop.setPhysicsEngine(&physics);
	
	// Set up main window
	w.setAudioSystem(audio);
	w.setGraphicsSystem(&renderer);
	w.setMainLoop(&game_loop);

	// Run this bitch
	return w.exec();
}