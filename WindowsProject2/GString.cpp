#include "GString.h"
#include "Windows.h"
#include <string>

GString::~GString()
{
	if (_array != nullptr)
		delete[] _array;
	_array = nullptr;
}

GString::GString(const char* c_str)
{
	_is_null = false;
	while (*c_str != '\0')
	{
		append(*c_str);
		++c_str;
	}
	append('\0');
}

GString::GString(const std::string& str)
{
	_is_null = false;
	const char* c_str = str.c_str();
	while (*c_str != '\0')
	{
		append(*c_str);
		++c_str;
	}
	append('\0');
}

GString::GString(const GString& other)
{
	*this = other;
}

GString::GString(GString&& other)
{
	_array = other._array;
	_count = other._count;
	_size = other._size;
	_is_null = other._is_null;

	other._array = nullptr;
	other._count = 0;
	other._size = 0;
	other._is_null = true;
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

	other._array = nullptr;
	other._count = 0;
	other._size = 0;
	other._is_null = true;

	return *this;
}

bool GString::operator==(const GString& other) const
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

GString GString::number(float f)
{
	return GString(std::to_string(f));
}