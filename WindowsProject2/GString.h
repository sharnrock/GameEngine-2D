#pragma once

#include <string>
#include "DynamicList.h"


// 'Good' String class
// ASCII string stuff, maybe I can create another for storing wide chars... it's Windows after all
class GString :
	protected DynamicList<char> 
{
public:
	GString() { _is_null = true; }
	~GString();
	GString(const char* c_str);
	GString(const std::string& str);
	
	// Deep copies
	GString(const GString& other);
	GString& operator=(const GString& other); 
	
	// Moves rvalues
	GString(GString&& other);
	GString& operator=(GString&& other);

	// TODO: implement this
	// converts the string into a hash.  Will be calculated once when the function is called
	// if the string changes, the hash will have to be recalculated
	unsigned int toHash() { return 0; }

	// This should compare hashes...
	bool operator==(const GString& other) const;
	int getStringSize() const;

	std::wstring toWideString() const;
	std::string  toStdString() const;

	static GString number(int i);
	static GString number(float f);

	bool isNull() const { return _is_null; }
	bool isEmpty() const { return DynamicList<char>::isEmpty(); }

private:
	bool _is_null;
};

