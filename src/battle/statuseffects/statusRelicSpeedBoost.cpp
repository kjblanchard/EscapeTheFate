#include <battle/statuseffects/statusEffects.hpp>
#include <gameobject/gameobjects/Battler.hpp>
using namespace Etf;

namespace {
bool applyFunc(StatusEffectInstance& s, Battler* b, Battler* t) {
	return true;
}

bool shouldPerform(StatusEffectInstance& s, Battler* b, Battler* t) {
	return s.Duration > 0;
}

bool actionFunc(StatusEffectInstance& s, Battler* b, Battler* t) {
	--s.Duration;
	if (s.Duration <= 0) {
		b->AddSpdBonus(-2);
	}
	return true;
}
}  // namespace

StatusEffectData Etf::RelicSpeedBoostEffect{
	applyFunc,
	shouldPerform,
	actionFunc,
	std::vector<StatusEffectTriggers>{StatusEffectTriggers::TurnEnd},
	StatusEffects::RelicSpeedBoost};
