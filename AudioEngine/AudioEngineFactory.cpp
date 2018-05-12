#include "stdafx.h"
//#include "AudioEngineFactory.h"
//#include "AudioBullshit.h"



AudioEngine * AudioEngineFactory::getAudioEngine()
{
	static AudioBullshit instance;
	return &instance;
}
