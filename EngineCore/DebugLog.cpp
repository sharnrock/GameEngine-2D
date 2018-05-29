#include "stdafx.h"
#include "DebugLog.h"


DebugLog::DebugLog() :
	output_file() // this should be set to null
{
}

DebugLog::~DebugLog()
{
}

DebugLog& DebugLog::Instance()
{
	static DebugLog instance;
	return instance;
}

// Set the file path for the log files; will not be written to until dumpLog msgs to file is called
void DebugLog::setOutputLogFile(const GString& file_path)
{
	output_file = file_path;
}

// Logs will not write to file until this is called
void DebugLog::dumpLogMsgsToFile()
{
	if (output_file.isNull())
	{
		error("Output log file not set call setOutputLogFile()");
	}
	else
	{
		OutputDebugString(TEXT("writing to log file not implemented yet"));
	}
}

void DebugLog::log(const GString& msg)
{
	GString prefix("\nLog:");
	prefix += msg;
	OutputDebugString(prefix.toWideString().c_str());
	//OutputDebugString(TEXT("\n"));
}

void DebugLog::warn(const GString& msg)
{
	GString prefix("Warn:");
	prefix += msg;
	OutputDebugString(prefix.toWideString().c_str());
	OutputDebugString(TEXT("\n"));
}

void DebugLog::error(const GString& msg)
{
	GString prefix("Error:");
	prefix += msg;
	OutputDebugString(prefix.toWideString().c_str());
	OutputDebugString(TEXT("\n"));
}
