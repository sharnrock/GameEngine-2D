#pragma once

#include "AudioEngine.h"
#include "XAudio2.h"
#include "DynamicList.h"
#include <map>


class AudioBullshit :
	public AudioEngine
{
	friend class VoiceCallback;
public:
	AudioBullshit();
	~AudioBullshit();

	HRESULT initialize();
	HRESULT uninitialize();

	void playSound(const GString& wav_file);
	void loadSoundFile(const GString& wav_file);
	void loadFilesInThisDir(const GString& audio_dir);

private:
	void sourceVoiceIsIdle(IXAudio2SourceVoice* idle_voice);
	void setMasterAudioFormat(const WAVEFORMATEX& format) { _master_format = format; }
	IXAudio2SourceVoice* getSourceVoice();

	IXAudio2*                         pXAudio2;
	IXAudio2MasteringVoice*           pMasterVoice;

	// Only needs to be set once, holds info like 44.1k 16bit yada, yada
	WAVEFORMATEX                      _master_format;
	DynamicList<IXAudio2SourceVoice*> _available_source_voices;
	DynamicList<IXAudio2SourceVoice*> _in_use_source_voices;
	std::map<UINT32, XAUDIO2_BUFFER*> _x2_buffers;

	DynamicList<VoiceCallback*>       _callbacks;
};



// Call back used to idle source voices
class VoiceCallback : public IXAudio2VoiceCallback
{
public:
	VoiceCallback(AudioBullshit* voice_container);
	void setSourceVoice(IXAudio2SourceVoice* voice) { this->voice = voice; }

	// called when the audio has finished playing
	void OnStreamEnd();

	void OnVoiceProcessingPassStart(UINT32 ) {}
	void OnVoiceProcessingPassEnd(void) {}
	void OnBufferStart(void * ) {}
	void OnBufferEnd(void * ) {}
	void OnLoopEnd(void * ) {}
	void OnVoiceError(void * , HRESULT ) {} // could probably be fed into log

	AudioBullshit*       getAudioEngine() { return voice_container; }
	IXAudio2SourceVoice* getSourceVoice() { return voice; }

private:
	AudioBullshit*       voice_container; 
	IXAudio2SourceVoice* voice;
};

