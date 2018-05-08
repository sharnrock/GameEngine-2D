#pragma once
#include <map>

template <class Key, class Value>
class GMap :
	public std::map<Key, Value>
{
public:
	bool contains(const Key& key);
};

template <class Key, class Value>
bool GMap<Key, Value>::contains(const Key& key)
{
	//std::count(key);
	//return true;
	// return (std::count(key));
}

