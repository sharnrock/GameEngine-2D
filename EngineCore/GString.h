#pragma once

#include "EngineCoreGlobal.h"
#include "DynamicList.h"
#include <string>
#include <basetsd.h>



// 'Good' String class
// GStrings are hashed as soon as they're created.  All logical comparisons operate on the hash
// so string comparisons can be done just as fast as integers assuming you're not creating the  
// string each time you do the comparison.  
// GStrings can be converted and used by windows by calling the toWideString().c_str() methods.
// Stores chars to conserve space.  
class DllExport GString :
	protected DynamicList<char> 
{
public:
	GString() : 
		DynamicList<char>(32),
		_is_null(true), 
		_hash(0) 
	{  
	}

	~GString();

	GString(const char* c_str);
	GString(const wchar_t* c_str);
	GString(const std::string& str);
	
	// Deep copies
	GString(const GString& other);
	GString& operator=(const GString& other); 
	
	// Moves rvalues
	GString(GString&& other);
	GString& operator=(GString&& other);

	// Concat 2 strings and store in lvalue
	GString& operator+=(const GString& add);
	GString operator+(const GString& add) const;
	

	// returns true if the string ends with the expression provided (for example ".txt" for filename)
	bool endsWith(const GString& end) const;

	// returns CRC32 hash of string buffer.  Used for logical comparisons
	UINT32 toHash() const;

	// String comparisons using hash value
	bool operator==(const GString& other) const;
	bool operator!=(const GString& other) const;
	bool operator<(const GString& other) const;
	bool operator>(const GString& other) const;
	bool operator>=(const GString & other) const;
	bool operator<=(const GString & other) const;

	// Actually compares the string contents, not just hash
	// Potentially useful for debugging hash collisions
	bool hasSameString(const GString& other) const;

	// Same as count, but this function name makes more sense for stings
	int getStringSize() const;

	std::wstring toWideString() const;
	std::string  toStdString() const;

	// Generate GStrings from numbers
	static GString number(int i);
	static GString number(float f);
	static GString number(UINT32 i);
	static GString number(__int64 i);

	// GString() is null and empty
	bool isNull() const { return _is_null; }

	// GString(""); is not null, but is empty
	bool isEmpty() const { return DynamicList<char>::isEmpty(); }

private:
	// runs the hash algorithm on the _array buffer and returns the answer to _hash
	void prepareHash();

	void convertWStringToCharPtr(std::wstring input, char ** outputString);

	// _array , _count, _size are inherited
	bool _is_null;
	UINT32 _hash;
};

