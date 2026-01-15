#pragma once
#include <string>
#include <Supergoon/Primitives/rectangle.h>
namespace Etf {
struct BattlerData {
	std::string Name, Sprite, IdleAnimation;
	int HP, Str, Mag, Def, MDef, Spd, Pow;
	RectangleF Location;
};

}  // namespace Etf
