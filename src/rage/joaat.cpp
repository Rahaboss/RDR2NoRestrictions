#include "pch.h"
#include "joaat.h"

rage::joaat_t rage::joaat(const std::string& str)
{
	rage::joaat_t hash = 0;

	for (char c : str)
	{
		hash += joaat_to_lower(c);
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}
