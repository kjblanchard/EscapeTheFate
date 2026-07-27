#pragma once
#include <string>
#include <Supergoon/Primitives/rectangle.h>
namespace Etf {
struct BattlerData {
	std::string Name, Sprite, IdleAnimation;
	int HP, Str, Mag, Def, MDef, Spd, Pow;
	int HpBarOffsetX = 0, HpBarOffsetY = 0;
	int AnimOffsetX = 0, AnimOffsetY = 0;
	int XPReward = 0;
	int CurrentXP = 0;
	int XPToNextLevel = 100;
	RectangleF Location;
};

}  // namespace Etf
