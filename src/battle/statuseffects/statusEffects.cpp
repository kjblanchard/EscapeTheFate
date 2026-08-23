#include <battle/statuseffects/statusEffects.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <map>
using namespace Etf;

namespace {
bool applyFunc(StatusEffectInstance& s, Battler* b, Battler* t) {
	return true;
}

bool shouldPerform(StatusEffectInstance& s, Battler* b, Battler* t) {
	return true;
}

bool actionFunc(StatusEffectInstance& s, Battler* b, Battler* t) {
	b->TakeDamage(1);
	--s.Duration;
	return true;
}

std::map<Etf::StatusEffects, StatusEffectData> statusEffects = {
	{StatusEffects::Poison, {applyFunc, shouldPerform, actionFunc, {StatusEffectTriggers::TurnEnd}, StatusEffects::Poison}}};
}  // namespace

void Etf::HandleStatusEffect(StatusEffectInstance& s, StatusEffectTriggers tr, Battler* b, Battler* t) {
	auto se = statusEffects.at(s.StatusType);

	StatusFunc func = nullptr;

	switch (tr) {
		case StatusEffectTriggers::Never:
			break;
		case StatusEffectTriggers::TurnStart:
			break;
		case StatusEffectTriggers::DamageTaken:
			break;
		case StatusEffectTriggers::TurnEnd:
			func = se.Action;
			break;
		case StatusEffectTriggers::Always:
			break;
	}
	func(s, b, t);
}
