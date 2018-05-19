#pragma once
#include <map>



template <class Key, class Value>
class GMap :
	public std::map<Key, Value>
{
public:
	bool contains(const Key& key) const;
};

// Returns true if key already exists
template <class Key, class Value>
bool GMap<Key, Value>::contains(const Key& key) const
{
	return std::map<Key, Value>::count(key);
}

