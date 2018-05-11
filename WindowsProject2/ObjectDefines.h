#pragma once

#include "GString.h"
#include <string> 

namespace Object
{
	enum OType
	{
		Undefined = 0,
		Robot,
		Slime,
		Projectile,
		BackGroundTileDisplayable
	};

	OType stringToOType(const std::string& str);
	GString oTypeToGString(OType type);
}
