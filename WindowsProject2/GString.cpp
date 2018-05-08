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
	while (*c_str != '\0')
	{
		append(*c_str);
		++c_str;
	}
	append('\0');
}

GString::GString(const std::string& str)
{
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

GString::GString(const GString&& other)
{

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