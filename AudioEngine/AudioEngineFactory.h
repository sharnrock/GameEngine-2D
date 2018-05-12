#pragma once

#include "AudioEngineGlobal.h"
class AudioEngine;

class DllExport AudioEngineFactory
{
public:
	static AudioEngine* getAudioEngine();
};

