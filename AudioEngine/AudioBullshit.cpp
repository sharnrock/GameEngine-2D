#include "stdafx.h"





// I don't know exactly what this thing does.  Maybe I can get rid of it or understand it better later
HRESULT FindMediaFileCch(_Out_writes_(cchDest) WCHAR* strDestPath, _In_ int cchDest, _In_z_ LPCWSTR strFilename);


AudioBullshit::AudioBullshit() :
	pXAudio2(nullptr),
	pMasterVoice(nullptr),
	hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL))
{
}

AudioBullshit::~AudioBullshit()
{
	CloseHandle(hBufferEndEvent);

	for (int i = 0; i < _available_source_voices.count(); i++)
	{
		_available_source_voices[i]->DestroyVoice();
	}

	for (int i = 0; i < _in_use_source_voices.count(); i++)
	{
		_in_use_source_voices[i]->Stop();
		_in_use_source_voices[i]->DestroyVoice();
	}

	if (pMasterVoice)
		pMasterVoice->DestroyVoice();
	
	// release this last
	if (pXAudio2)
		pXAudio2->Release();
}

HRESULT AudioBullshit::init()
{
	HRESULT hr = S_OK;
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		return hr;

	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
		return hr;
	return hr;
}

void AudioBullshit::playSound(const GString& wav_file)
{
	GString msg = GString("\nPlaying: ") + wav_file;
	OutputDebugString(msg.toWideString().c_str());

	IXAudio2SourceVoice *pSourceVoice = getSourceVoice();

	HRESULT hr = S_OK;

	XAUDIO2_BUFFER *buffer_ptr = _x2_buffers[wav_file.toHash()];
	XAUDIO2_BUFFER &buffer = *buffer_ptr;

	GString msg2("Playing Hash:");
	msg2 += GString::number(wav_file.toHash());
	OutputDebugString(msg2.toWideString().c_str());

	if (!buffer_ptr)
		return;

	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
	{
		wprintf(L"Error %#X submitting source buffer\n", hr);
		return;
	}

	hr = pSourceVoice->Start(0);
	return;
}

void AudioBullshit::loadSoundFile(const GString& wav_file)
{
	WCHAR strFilePath[MAX_PATH];
	HRESULT hr = FindMediaFileCch(strFilePath, MAX_PATH, wav_file.toWideString().c_str());
	if (FAILED(hr))
	{
		wprintf(L"Failed to find media file: %s\n", wav_file.toWideString().c_str());
		return;
	}

	// Read in the wave file
	std::unique_ptr<uint8_t[]>* waveFile = new std::unique_ptr<uint8_t[]>();
	DirectX::WAVData waveData;
	if (FAILED(hr = DirectX::LoadWAVAudioFromFileEx(strFilePath, *waveFile, waveData)))
	{
		wprintf(L"Failed reading WAV file: %#X (%s)\n", hr, strFilePath);
		return;
	}
	
	setMasterAudioFormat(*waveData.wfx);
	
	// Submit the wave sample data using an XAUDIO2_BUFFER structure
	// Create a new instance of this thing, but turn it into an alias so I don't have to change a bunch of code
	XAUDIO2_BUFFER *buffer_ptr = new XAUDIO2_BUFFER;
	XAUDIO2_BUFFER &buffer = *buffer_ptr;
	_x2_buffers[wav_file.toHash()] = buffer_ptr;

	GString msg("Loading Hash:");
	msg += GString::number(wav_file.toHash());
	OutputDebugString(msg.toWideString().c_str());

	buffer = { 0 };
	buffer.pAudioData = waveData.startAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
	buffer.AudioBytes = waveData.audioBytes;

	if (waveData.loopLength > 0)
	{
		buffer.LoopBegin = waveData.loopStart;
		buffer.LoopLength = waveData.loopLength;
		buffer.LoopCount = 1; // We'll just assume we play the loop twice
	}

	return;
}

void AudioBullshit::loadFilesInThisDir(const GString& audio_dir)
{
	GDir dir(audio_dir);
	DynamicList<GString> files = dir.getFileList();
	for (int i = 0; i < files.count(); i++)
	{
		GString msg = GString("\nLoading: ") + audio_dir + files.at(i);
		OutputDebugString(msg.toWideString().c_str());
		if (files.at(i).endsWith(".wav"))
			loadSoundFile(audio_dir + files.at(i));
	}
}

IXAudio2SourceVoice* AudioBullshit::getSourceVoice()
{
	HRESULT hr = S_OK;
	// Create the source voice

	
	if (_available_source_voices.isEmpty())
	{
		// TODO: find a way to use the callbacks, probably going to have to create some command pattern type class that 
		// takes in the object pool stuff and handles all that... 
		// later...
		
		// we create it..
		IXAudio2SourceVoice *pSourceVoice;
		if (FAILED(hr = this->pXAudio2->CreateSourceVoice(&pSourceVoice, &_master_format))) // old one
		//if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, &_master_format,
		//	0, XAUDIO2_DEFAULT_FREQ_RATIO, this, NULL, NULL)))
		{
			wprintf(L"Error %#X creating source voice\n", hr);
			return nullptr;
		}
		_in_use_source_voices.append(pSourceVoice);
		return pSourceVoice;
	}
	else
	{
		IXAudio2SourceVoice *pSourceVoice = _available_source_voices.popBack();
		_in_use_source_voices.append(pSourceVoice);
		return pSourceVoice;
	}
}















//--------------------------------------------------------------------------------------
// Helper function to try to find the location of a media file
//--------------------------------------------------------------------------------------
_Use_decl_annotations_
HRESULT FindMediaFileCch(WCHAR* strDestPath, int cchDest, LPCWSTR strFilename)
{
	bool bFound = false;

	if (!strFilename || strFilename[0] == 0 || !strDestPath || cchDest < 10)
		return E_INVALIDARG;

	// Get the exe name, and exe path
	WCHAR strExePath[MAX_PATH] = { 0 };
	WCHAR strExeName[MAX_PATH] = { 0 };
	WCHAR* strLastSlash = nullptr;
	GetModuleFileName(nullptr, strExePath, MAX_PATH);
	strExePath[MAX_PATH - 1] = 0;
	strLastSlash = wcsrchr(strExePath, TEXT('\\'));
	if (strLastSlash)
	{
		wcscpy_s(strExeName, MAX_PATH, &strLastSlash[1]);

		// Chop the exe name from the exe path
		*strLastSlash = 0;

		// Chop the .exe from the exe name
		strLastSlash = wcsrchr(strExeName, TEXT('.'));
		if (strLastSlash)
			*strLastSlash = 0;
	}

	wcscpy_s(strDestPath, cchDest, strFilename);
	if (GetFileAttributes(strDestPath) != 0xFFFFFFFF)
		return S_OK;

	// Search all parent directories starting at .\ and using strFilename as the leaf name
	WCHAR strLeafName[MAX_PATH] = { 0 };
	wcscpy_s(strLeafName, MAX_PATH, strFilename);

	WCHAR strFullPath[MAX_PATH] = { 0 };
	WCHAR strFullFileName[MAX_PATH] = { 0 };
	WCHAR strSearch[MAX_PATH] = { 0 };
	WCHAR* strFilePart = nullptr;

	GetFullPathName(L".", MAX_PATH, strFullPath, &strFilePart);
	if (!strFilePart)
		return E_FAIL;

	while (strFilePart && *strFilePart != '\0')
	{
		swprintf_s(strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName);
		if (GetFileAttributes(strFullFileName) != 0xFFFFFFFF)
		{
			wcscpy_s(strDestPath, cchDest, strFullFileName);
			bFound = true;
			break;
		}

		swprintf_s(strFullFileName, MAX_PATH, L"%s\\%s\\%s", strFullPath, strExeName, strLeafName);
		if (GetFileAttributes(strFullFileName) != 0xFFFFFFFF)
		{
			wcscpy_s(strDestPath, cchDest, strFullFileName);
			bFound = true;
			break;
		}

		swprintf_s(strSearch, MAX_PATH, L"%s\\..", strFullPath);
		GetFullPathName(strSearch, MAX_PATH, strFullPath, &strFilePart);
	}
	if (bFound)
		return S_OK;

	// On failure, return the file as the path but also return an error code
	wcscpy_s(strDestPath, cchDest, strFilename);

	return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
}



