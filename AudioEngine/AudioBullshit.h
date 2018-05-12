#pragma once

//#include <Windows.h>
#include "XAudio2.h"
#include "AudioEngine.h"
#include <wrl\client.h>
//#include "comptr.h"
//#include <wrl.h>

class AudioBullshit :
	public AudioEngine
{
public:
	AudioBullshit();
	~AudioBullshit();

	HRESULT init();
	HRESULT initMasterVoice();

	void loadTestSound();
	void playTestSound();

private:

	//Microsoft::WRL::ComPtr<IXAudio2> pXAudio2;
	//ComPtr<IXAudio2> pXAudio2;
	 IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasterVoice;
};

