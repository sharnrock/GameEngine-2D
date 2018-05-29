#include "stdafx.h"



GString::~GString()
{
	if (_array != nullptr)
		delete[] _array;
	_array = nullptr;
}

GString::GString(const char* c_str) :
	DynamicList<char>(32)
{
	_is_null = false;
	while (*c_str != '\0')
	{
		append(*c_str);
		++c_str;
	}
	append('\0');

	prepareHash(); 
}

void GString::convertWStringToCharPtr(std::wstring input, char ** outputString)
{
	size_t outputSize = 1;
	while (outputSize < input.length() + 1)
		outputSize <<= 1; // double

	*outputString = new char[outputSize];
	size_t charsConverted = 0;
	wcstombs_s(&charsConverted, *outputString, outputSize, input.c_str(), input.length());
	_count = (int)charsConverted;
	_size  = (int)outputSize;
}

GString::GString(const wchar_t* c_str) :
	DynamicList<char>(32),
	_is_null(false)
{
	if (_array)
		delete[] _array;

	convertWStringToCharPtr(c_str, &_array);
	prepareHash();
}

GString::GString(const std::string& str) :
	DynamicList<char>(32)
{
	_is_null = false;
	const char* c_str = str.c_str();
	while (*c_str != '\0')
	{
		append(*c_str);
		++c_str;
	}
	append('\0');

	prepareHash();
}

GString::GString(const GString& other) :
	DynamicList<char>(32)
{
	*this = other;
}

GString::GString(GString&& other)
{
	_array = other._array;
	_count = other._count;
	_size = other._size;
	_is_null = other._is_null;
	_hash = other._hash;

	other._array = nullptr;
	other._count = 0;
	other._size = 0;
	other._is_null = true;
	other._hash = 0;
}

GString& GString::operator=(const GString& other)
{ 
	if (this->_array != nullptr)
		delete[] this->_array;

	_array = new char[other._size];
	this->_size = other._size;

	char* ptr = other._array;
	char* ths = this->_array;
	while (*ptr != '\0')
	{
		*ths = *ptr;
		++ths;
		++ptr;
	}
	*ths = '\0';

	this->_count = other._count;
	this->_is_null = other._is_null;
	this->_hash = other._hash;
	return *this;
}

GString& GString::operator=(GString&& other)
{
	if (this == &other)
		return *this;

	if (_array != nullptr)
		delete[] _array;

	_array = other._array;
	_count = other._count;
	_size = other._size;
	_is_null = other._is_null;
	_hash = other._hash;

	other._array = nullptr;
	other._count = 0;
	other._size = 0;
	other._is_null = true;
	other._hash = 0;

	return *this;
}

GString& GString::operator+=(const GString& add)
{
	--_count; // remove null pointer
	for (int i = 0; i < add.count(); i++)
	{
		char a = add.at(i);
		append(add.at(i));
	}
	this->prepareHash();
	return *this;
}

GString GString::operator+(const GString& add) const
{
	GString result = *this;
	result += add;
	return result;
}

bool GString::endsWith(const GString& end) const
{
	int offset = count() - end.count();
	for (int i = 0; i < end.count(); i++)
	{
		if (_array[i+offset] != end._array[i])
			return false;
	}

	return true;
}

UINT32 GString::toHash() const
{
	return _hash;
}

void GString::prepareHash()
{
	UINT32 starter = ~0U; // I don't know what this is supposed to be, but it'll do for now
	_hash = hash::calculate_crc32c(starter, (const UINT8*)_array, (UINT32)_count);
}

bool GString::operator==(const GString& other) const
{
	return this->toHash() == other.toHash();
}

bool GString::operator!=(const GString& other) const
{
	return !(*this == other);
}

bool GString::operator<(const GString& other) const
{
	return this->toHash() < other.toHash();
}

bool GString::operator>(const GString& other) const
{
	return this->toHash() > other.toHash();
}

bool GString::operator>=(const GString& other) const
{
	return this->toHash() >= other.toHash();
}

bool GString::operator<=(const GString& other) const
{
	return this->toHash() <= other.toHash();
}

bool GString::hasSameString(const GString& other) const
{
	if (this->_count != other._count)
		return false;

	for (int i = 0; i < this->_count; i++)
	{
		if (this->_array[i] != other._array[i])
			return false;
	}

	return true;
}

std::wstring GString::toWideString() const
{
	int len = MultiByteToWideChar(CP_ACP, 0, _array, count(), 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, _array, count(), buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

std::string GString::toStdString() const
{
	return std::string(_array);
}

int GString::getStringSize() const
{
	return count();
}

GString GString::number(int i)
{
	return GString(std::to_string(i));
}

GString GString::number(UINT32 i)
{
	return GString(std::to_string(i));
}

GString GString::number(float f)
{
	return GString(std::to_string(f));
}

GString GString::number(__int64 i)
{
	return GString(std::to_string(i));
}

