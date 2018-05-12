#pragma once

//#include <Windows.h>

//#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\comdecl.h"
//#include "XAudio2.h"

//#include "audio\comdecl.h"
//#include "XAudio2.h"

#include "AudioEngineGlobal.h"

class DllExport AudioEngine
{
public:
	AudioEngine();
	virtual ~AudioEngine();

	virtual HRESULT init() = 0;
	virtual HRESULT initMasterVoice() = 0;
	virtual void loadTestSound() = 0;
	virtual void playTestSound() = 0;
	void thing();
};
