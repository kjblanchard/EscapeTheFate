#include <battle/enemyAI.hpp>

#include <cstdlib>
#include <gameobject/gameobjects/Battler.hpp>
#include <systems/battleSystem.hpp>

using namespace Etf;

AIAction BasicEnemyAI::SelectAction(Battler* self, const std::vector<Battler*>& allies, const std::vector<Battler*>& enemies) {
	auto* data = self->GetBattlerData();
	if (data->Abilities.empty() || enemies.empty()) return {};

	// Pick a random ability we can afford
	std::vector<int> affordable;
	for (int id : data->Abilities) {
		if (BattleSystem::HasAbility(id)) {
			const auto& ability = BattleSystem::GetAbilityByID(id);
			if (self->CurrentAP() >= ability.APCost) {
				affordable.push_back(id);
			}
		}
	}
	if (affordable.empty()) return {};

	int chosenID = affordable[rand() % affordable.size()];
	const auto& ability = BattleSystem::GetAbilityByID(chosenID);

	// Pick a random target from the appropriate pool
	Battler* target = nullptr;
	if (ability.Friendly) {
		if (!allies.empty()) target = allies[rand() % allies.size()];
	} else {
		target = enemies[rand() % enemies.size()];
	}

	return {chosenID, target};
}

EnemyAIStrategy* Etf::CreateEnemyAI(const std::string& name) {
	// Default or "basic" both use BasicEnemyAI
	// Add new strategies here as needed:
	// if (name == "aggressive") return new AggressiveEnemyAI();
	return new BasicEnemyAI();
}
