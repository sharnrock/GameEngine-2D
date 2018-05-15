#pragma once


#include "AudioEngineGlobal.h"
class GString;



class DllExport AudioEngine
{
public:
	virtual ~AudioEngine();

	// Must be called after instantiation and before any other function calls
	virtual HRESULT init() = 0;

	// Loads a sound file into memory for continual reuse
	virtual void loadSoundFile(const GString&) = 0;
	virtual void loadFilesInThisDir(const GString&) = 0;
	// Just play a sound in general; could be background music or something
	virtual void playSound(const GString& wav_file) = 0;

};
