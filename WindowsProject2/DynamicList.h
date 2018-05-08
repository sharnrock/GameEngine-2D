#pragma once

#include <assert.h>

/**
Will create an array on the heap that will keep track of item count
*/
template <class T>
class DynamicList
{
public:
	// Allocate an array of this many elements on the heap
	DynamicList(int prealloc_size = 1);
	virtual ~DynamicList();

	DynamicList(const DynamicList<T>&);
	DynamicList(const DynamicList<T>&&);

	// Size of allocated space
	int size() const { return _size; }

	// Element count
	int count() const { return _count; }
	
	// Assignment operator
	DynamicList<T>& operator=(const DynamicList<T>& other);

	// Get element reference
	T& operator[](int ndx);

	// Returns true if contents within count are equal
	bool operator==(const DynamicList<T>& other) const;

	// Get element const reference
	const T& at(int ndx) const;

	// Add to the end of the list
	void append(const T& addition);

	// Remove a random element and shift the remaining elements in its place
	void removeAt(int ndx);

	// remove all elements
	void clear();

	// returns true if the value is contained in the list
	bool contains(const T& value) const;

	// grab the element at ndx 0 or right before count
	T& front() { return (*this)[0]; }
	T& back() { return (*this)[_count-1]; }

	bool isEmpty() const { return !_count; }

protected:
	void doubleSize();

	T * _array;
	int _count;
	int _size;
};



template <class T>
DynamicList<T>::DynamicList(int prealloc_size) :
	_array(nullptr),
	_count(0),
	_size(prealloc_size)
{
	_array = new T[_size];
}

template <class T>
DynamicList<T>::~DynamicList()
{
	if (_array)
		delete[] _array;
	_array = nullptr;
}

template <class T>
DynamicList<T>& DynamicList<T>::operator=(const DynamicList<T>& other)
{
	if (this->_array != nullptr)
		delete[] this->_array;

	this->_array = new T[other._size];
	this->_size = other._size;
	
	for (int i = 0; i < other._count; i++)
	{
		this->_array[i] = other._array[i];
	}
	this->_count = other._count;

	return *this;
}

template <class T>
DynamicList<T>::DynamicList(const DynamicList<T>& other)
{
	*this = other;
}

template <class T>
DynamicList<T>::DynamicList(const DynamicList<T>&& other)
{
	if (this->_array)
		delete[] _array;
	this->_array = other._array;
	other._array = null;
	
	this->_count = other._count;
	this->_size = other._size;
}

template <class T>
T& DynamicList<T>::operator[](int ndx)
{
	assert(ndx < _count);
	return _array[ndx];
}

template <class T>
bool DynamicList<T>::operator==(const DynamicList<T>& other) const
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

template <class T>
const T& DynamicList<T>::at(int ndx) const
{
	assert(ndx < _count);
	return _array[ndx];
}

template <class T>
void DynamicList<T>::append(const T& addition)
{
	if (_count == _size)
		doubleSize();
	_array[_count] = addition;
	++_count;
}

template <class T>
void DynamicList<T>::removeAt(int ndx)
{
	--_count;
	// Just shift everything on top of the ndx we're removing
	for (int i = ndx; i < _count; i++)
		_array[i] = _array[i + 1];
}

template <class T>
void DynamicList<T>::doubleSize()
{
	_size <<= 1; // multiply by 2
	T* new_array = new T[_size];
	
	// move contents from old array into new array space
	for (int i = 0; i < _count; i++)
		new_array[i] = _array[i];

	delete[] _array;
	_array = new_array;
}

template <class T>
bool DynamicList<T>::contains(const T& value) const
{
	for (int i = 0; i < _count; i++)
	{
		if (_array[i] == value)
			return true;
	}
	return false;
}

template <class T>
void DynamicList<T>::clear()
{
	_count = 0;
}
