#include "ObjectDefines.h"

using namespace Object;


OType Object::stringToOType(const std::string & str)
{
	if (str == "Robot")
		return Robot;
	else if (str == "Slime")
		return Slime;
	else if (str == "Projectile")
		return Projectile;
	else if (str == "BackGroundTileDisplayable")
		return BackGroundTileDisplayable;
	else
		return Undefined;
}

GString Object::oTypeToGString(OType type)
{
	switch (type)
	{
	case Robot:
		return "Robot";
	case Slime:
		return "Slime";
	case Projectile:
		return "Projectile";
	case BackGroundTileDisplayable:
		return "BackGroundTileDisplayable";
	case Undefined:
	default:
		return "Undefined";
	}
}
