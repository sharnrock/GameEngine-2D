#include "stdafx.h"



GDir::GDir(const GString& dir_path) :
	_dir_path(dir_path)
{
}

GDir::~GDir()
{
}

DynamicList<GString> GDir::getFileList() const
{
	DynamicList<GString> result;
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	// Set up the string so it'll actually work in the craphole window's function
	GString dir_path = _dir_path;
	if (!dir_path.endsWith("\\*"))
		dir_path += "\\*";

	hFind = FindFirstFile(dir_path.toWideString().c_str(), &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		return result;
	}

	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			result.append(ffd.cFileName);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		return result;
	}

	FindClose(hFind);
	return result;
}
