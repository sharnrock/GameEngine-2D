#include "stdafx.h"
//#include "AudioEngine.h"
//#include "AudioBullshit.h"





AudioEngine::AudioEngine() 
{
	
}


AudioEngine::~AudioEngine()
{

}

void AudioEngine::thing()
{
	AudioBullshit bs;
	bs.init();
	bs.initMasterVoice();
}


