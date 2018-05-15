#pragma once

#include "EngineCoreGlobal.h"
#include "DynamicList.h"
#include "GString.h"

class DllExport GDir
{
public:
	GDir(const GString& dir_path);
	~GDir();

	DynamicList<GString> getFileList() const;

private:
	GString _dir_path;
};

