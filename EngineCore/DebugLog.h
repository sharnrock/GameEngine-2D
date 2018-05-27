#pragma once

#include "GString.h"
#include "EngineCoreGlobal.h"

class DllExport DebugLog
{
public:
	~DebugLog();

	static DebugLog& Instance();

	// Set the file path for the log files; will not be written to until dumpLog msgs to file is called
	void setOutputLogFile(const GString& file_path);

	// Logs will not write to file until this is called
	void dumpLogMsgsToFile();

	void log(const GString& msg);
	void warn(const GString& msg);
	void error(const GString& msg);

private:
	GString output_file;

	// Singleton Stuff
private:
	DebugLog();
	DebugLog(const DebugLog&) = delete;
	DebugLog(const DebugLog&&) = delete;
	DebugLog& operator=(const DebugLog&) = delete;
	DebugLog& operator=(const DebugLog&&) = delete;
};

