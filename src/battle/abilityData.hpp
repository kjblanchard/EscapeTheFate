#pragma once
#include <string>
namespace Etf {

enum class AbilityDamageType { Physical, Magic };

struct AbilityData {
	std::string Name;
	std::string AnimationFile;
	std::string AnimationTag;
	std::string SFXName;
	std::string PlayerAnim;
	int BaseDamage = 1;
	bool Friendly = false;
	AbilityDamageType DamageType = AbilityDamageType::Physical;
};
}  // namespace Etf
