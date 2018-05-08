#pragma once

#include <string>
#include "DynamicList.h"

// 'Good' String class
// ASCII string stuff
class GString :
	protected DynamicList<char> // TODO: change this class to use wchar_t
{
public:
	GString() {}
	~GString();
	GString(const char* c_str);
	GString(const std::string& str);

	GString(const GString& other);
	GString(const GString&& other);
	
	GString& operator=(const GString& other); 
	bool operator==(const GString& other) const;

	int getStringSize() const;

	std::wstring toWideString() const;
	std::string toStdString() const;

	static GString number(int i);
	static GString number(float f);
};

