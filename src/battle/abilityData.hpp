#pragma once
#include <string>
namespace Etf {
struct AbilityData {
	std::string Name;
	std::string AnimationFile;
	std::string AnimationTag;
	std::string SFXName;
	std::string PlayerAnim;
	int BaseDamage = 1;
	bool Friendly = false;
};
}  // namespace Etf
