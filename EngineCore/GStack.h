#pragma once

#include "DynamicList.h"
#include "EngineCoreGlobal.h"

// This class doesn't do much, but it's a little more pedantic(??) than the dynamic list when it comes to stack like behavior
template <class T>
class DllExport GStack :
	public DynamicList<T>
{
public:
	GStack(int size = 32);
	void push(T&);
	T& pop();
};

template <class T>
GStack<T>::GStack(int size) :
	DynamicList<T>(size)
{
}

template <class T>
void GStack<T>::push(T& element)
{
	this->append(element);
}

template <class T>
T& GStack<T>::pop()
{
	return this->popBack();
}