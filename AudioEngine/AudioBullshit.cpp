#include "stdafx.h"




// I don't know exactly what this thing does.  Maybe I can get rid of it or understand it better later
HRESULT FindMediaFileCch(_Out_writes_(cchDest) WCHAR* strDestPath, _In_ int cchDest, _In_z_ LPCWSTR strFilename);

// temp globals
// This guy has to stay around..
std::unique_ptr<uint8_t[]> waveFile;

AudioBullshit::AudioBullshit() :
	pXAudio2(nullptr),
	pMasterVoice(nullptr),
	hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL))
{
}

AudioBullshit::~AudioBullshit()
{
	CloseHandle(hBufferEndEvent);
	// Take care of these
	/*IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasterVoice;

	DynamicList<XAUDIO2_BUFFER*> _x2_buffers;
	DynamicList<IXAudio2SourceVoice*> _source_voices;
	DynamicList< std::unique_ptr<uint8_t[]>* > _wav_file_data;*/
	

	//for (int i = 0; i < _wav_file_data.count(); i++)
	//{
	//	_wav_file_data[i]->release();
	//	delete _wav_file_data[i];
	//}

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

void AudioBullshit::loadTestSound()
{
	// this is all going away
	loadSoundFile("C:\\Users\\Keith\\source\\repos\\WindowsProject2\\Assets\\Audio\\shoot.wav");
	loadSoundFile("C:\\Users\\Keith\\source\\repos\\WindowsProject2\\Assets\\Audio\\jump.wav");
}

void AudioBullshit::playTestSound()
{
	playSound("C:\\Users\\Keith\\source\\repos\\WindowsProject2\\Assets\\Audio\\jump.wav");
}

void AudioBullshit::playSound(const GString& wav_file)
{
	IXAudio2SourceVoice *pSourceVoice = getSourceVoice();

	HRESULT hr = S_OK;


	XAUDIO2_BUFFER *buffer_ptr = _x2_buffers[wav_file.toHash()];
	XAUDIO2_BUFFER &buffer = *buffer_ptr;




	// This part should be in play sound!!!!!!!!
	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
	{
		wprintf(L"Error %#X submitting source buffer\n", hr);
		//pSourceVoice->DestroyVoice();
		return;
	}


	hr = pSourceVoice->Start(0);
	return;

	// do this eventually somewhere..
	//pSourceVoice->DestroyVoice();







}

void AudioBullshit::loadSoundFile(const GString& wav_file)
{
	// TODO: yank out the source creation so it can object pool instead
	// Keep the loading of buffer and wav_file in

	// S1 find file
	// S2 create voice - take this out and create object pool
	// S3 create buffer
	// S4 load buffer in to voice - Probably take this out and come up with more complicated system
	// S5 call start to play it - Create something totally different to start playback
	
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
	//_wav_file_data = (std::unique_ptr<uint8_t[]>&&)waveFile;
	//_wav_file_data.append(waveFile); 
	setMasterAudioFormat(*waveData.wfx);
	// This is pretty much the point where we can Stop..
	// Let some other piece of the program take over the handling of source voices
	// actually, we still need to create the buffer



	// Create the source voice
#if 0

	IXAudio2SourceVoice *pSourceVoice = getSourceVoice();
	if (FAILED(hr = this->pXAudio2->CreateSourceVoice(&pSourceVoice, waveData.wfx)))
	{
		wprintf(L"Error %#X creating source voice\n", hr);
		return;
	}
	_source_voices.append(pSourceVoice);
#endif


	// Submit the wave sample data using an XAUDIO2_BUFFER structure
	// Create a new instance of this thing, but turn it into an alias so I don't have to change a bunch of code
	XAUDIO2_BUFFER *buffer_ptr = new XAUDIO2_BUFFER;
	XAUDIO2_BUFFER &buffer = *buffer_ptr;
	_x2_buffers[wav_file.toHash()] = buffer_ptr;
	//_x2_buffers.append(buffer_ptr);

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
	OutputDebugString(TEXT("AudioBullshit::loadFilesInThisDir: Not yet implemented"));
	return;
#if 0
	WIN32_FIND_DATA ffd;
	//LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	//size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	//DWORD dwError = 0;
	FindFirstFile(szDir, &ffd);
		//if (INVALID_HANDLE_VALUE == hFind)

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	std::wstring msg(L"Trying to load asset dir");
	msg += audio_dir.toWideString();
	OutputDebugString(msg.c_str());
#endif

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



