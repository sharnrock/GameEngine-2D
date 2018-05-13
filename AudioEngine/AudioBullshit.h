#pragma once

#include "AudioEngine.h"
#include "XAudio2.h"
#include "DynamicList.h"
#include "GMap.h"
#include <memory>

class AudioBullshit :
	public AudioEngine,
	public IXAudio2VoiceCallback
{
public:
	AudioBullshit();
	~AudioBullshit();

	HRESULT init(); 

	// Currently loads all the sounds needed by the program..
	// This will change eventually
	void loadTestSound();
	// Plays the jump sound once
	void playTestSound();

	void playSound(const GString& wav_file);
	void loadSoundFile(const GString& wav_file);

	// Not yet implemented..
	void loadFilesInThisDir(const GString& audio_dir);


// 
// Call back stuff for voices
//
public:
	HANDLE hBufferEndEvent;

	// This is the one we want
	void OnStreamEnd() { SetEvent(hBufferEndEvent); }

	//Unused methods are stubs
	void OnVoiceProcessingPassEnd() { }
	void OnVoiceProcessingPassStart(UINT32) {    }
	void OnBufferEnd(void *) {  } 
	void OnBufferStart(void *) {    }
	void OnLoopEnd(void *) {    }
	void OnVoiceError(void *, HRESULT Error) { }
// 
// Call back stuff for voices
//



private:
	void setMasterAudioFormat(const WAVEFORMATEX& format) { _master_format = format; }
	IXAudio2SourceVoice* getSourceVoice();

	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasterVoice;
	WAVEFORMATEX _master_format;

	DynamicList<IXAudio2SourceVoice*> _available_source_voices;
	DynamicList<IXAudio2SourceVoice*> _in_use_source_voices;

	GMap<UINT32, XAUDIO2_BUFFER*> _x2_buffers;
};
// This might need to persist... I don't know.  I'm taking it out, but if the audio starts to 
// buckle, put it back in to see if that fixes it
//DynamicList< std::unique_ptr<uint8_t[]>* > _wav_file_data;



#if 0
class VoiceCallback : public IXAudio2VoiceCallback
{
public:
	HANDLE hBufferEndEvent;
	VoiceCallback() : hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
	~VoiceCallback() { CloseHandle(hBufferEndEvent); }

	//Called when the voice has just finished playing a contiguous audio stream.
	void OnStreamEnd() { SetEvent(hBufferEndEvent); }

	//Unused methods are stubs
	void OnVoiceProcessingPassEnd() { }
	void OnVoiceProcessingPassStart(UINT32 SamplesRequired) {    }
	void OnBufferEnd(void * pBufferContext) { }
	void OnBufferStart(void * pBufferContext) {    }
	void OnLoopEnd(void * pBufferContext) {    }
	void OnVoiceError(void * pBufferContext, HRESULT Error) { }
};
#endif
