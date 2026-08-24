#pragma once
#include <string>
#include <vector>
namespace Etf {
struct StatusEffectChance {
	int Id;
	float ApplyChance;
};

struct AbilityData {
	std::string Name;
	std::string AnimationFile;
	std::string AnimationTag;
	std::string SFXName;
	std::string PlayerAnim;
	int BaseDamage = 1;
	bool Friendly = false;
	int APCost = 1;
	std::string Description;
	std::vector<StatusEffectChance> StatusEffects;
};
}  // namespace Etf
