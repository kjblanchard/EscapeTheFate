#include <battle/statuseffects/statusEffects.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <map>
using namespace Etf;

namespace {
std::map<Etf::StatusEffects, StatusEffectData> statusEffectsMap = {
	{StatusEffects::Poison, Poison},
	{StatusEffects::RelicDamageBonus, RelicDamageBonusEffect},
	{StatusEffects::RelicSpeedBoost, RelicSpeedBoostEffect},
	{StatusEffects::RelicShield, RelicShieldEffect},
};
}  // namespace

void Etf::HandleStatusEffect(StatusEffectInstance& s, StatusEffectTriggers tr, Battler* b, Battler* t) {
	auto it = statusEffectsMap.find(s.StatusType);
	if (it == statusEffectsMap.end()) return;
	auto& se = it->second;

	StatusFunc func = nullptr;

	switch (tr) {
		case StatusEffectTriggers::Never:
			break;
		case StatusEffectTriggers::TurnStart:
			func = se.Action;
			break;
		case StatusEffectTriggers::DamageTaken:
			func = se.Action;
			break;
		case StatusEffectTriggers::TurnEnd:
			func = se.Action;
			break;
		case StatusEffectTriggers::Always:
			func = se.Action;
			break;
	}
	if (func) func(s, b, t);
}
