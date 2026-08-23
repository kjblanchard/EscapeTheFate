#pragma once
#include <vector>

namespace Etf {

class Battler;

enum class StatusEffectTriggers {
	Never,
	TurnStart,
	DamageTaken,
	TurnEnd,
	Always,
};

enum class StatusEffects {
	None,
	Poison,
	Slow,
	Haste,
	Defend,
	Critical,
	RelicDamageBonus,
	RelicSpeedBoost,
	RelicShield,
};

struct StatusEffectInstance {
	StatusEffects StatusType;
	int Duration;
};

using StatusFunc = bool (*)(StatusEffectInstance&, Battler*, Battler*);

struct StatusEffectData {
	StatusFunc ApplyFunc;
	StatusFunc ShouldPerformFunc;
	StatusFunc Action;
	std::vector<StatusEffectTriggers> Triggers;
	StatusEffects Type;
	// inline bool ShouldPerform(StatusEffectTriggers tr) const { return isTriggerType(tr) && shouldPerform(); }
	// inline bool isTriggerType(StatusEffectTriggers tr) const { return std::find(triggers.begin(), triggers.end(), tr) != triggers.end(); }
};

// Declare all status effects so we can add them to map
extern StatusEffectData Poison;
extern StatusEffectData RelicDamageBonusEffect;
extern StatusEffectData RelicSpeedBoostEffect;
extern StatusEffectData RelicShieldEffect;

// Perform status effect s at a specific trigger tr on battler b against battler t, target can be null
void HandleStatusEffect(StatusEffectInstance& s, StatusEffectTriggers tr, Battler* b, Battler* t);
// 	if (s.ShouldPerform(tr)) s.Action(b, t);
// }

}  // namespace Etf
