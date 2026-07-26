#pragma once
#include <string>
namespace Etf {
struct AbilityData {
	std::string Name;
	std::string AnimationFile;
	std::string AnimationTag;
	std::string SFXName;
	int BaseDamage = 1;
};
}  // namespace Etf
