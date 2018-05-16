#include "stdafx.h"
#include "GFile.h"

#include <fstream>

GFile::GFile(const GString& filepath) :
	_file_path(filepath)
{
}


GFile::~GFile()
{
}


std::string GFile::getFileAsString()
{
	std::string result = "";
	std::string line;
	std::ifstream myfile(_file_path.toStdString());

	if (!myfile.is_open())
		return result;

	while (getline(myfile, line))
	{
		result += line;
	}
	myfile.close();

	return result;
}