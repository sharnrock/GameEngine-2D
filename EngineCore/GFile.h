#pragma once

#include "EngineCoreGlobal.h"
#include <string>

// not the best implementation, but I needed something quick..
class DllExport GFile
{
public:
	GFile(const GString& filepath);
	~GFile();

	std::string getFileAsString();

private:
	GString _file_path;
};

