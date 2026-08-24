#include <battle/statuseffects/statusEffects.hpp>
#include <gameobject/gameobjects/Battler.hpp>
using namespace Etf;

namespace {
bool applyFunc(StatusEffectInstance& s, Battler* b, Battler* t) {
	return true;
}

bool shouldPerform(StatusEffectInstance& s, Battler* b, Battler* t) {
	return false;
}

bool actionFunc(StatusEffectInstance& s, Battler* b, Battler* t) {
	return true;
}
}  // namespace

StatusEffectData Etf::RelicShieldEffect{
	applyFunc,
	shouldPerform,
	actionFunc,
	std::vector<StatusEffectTriggers>{StatusEffectTriggers::Never},
	StatusEffects::RelicShield};
