#pragma once
#include <battle/abilityData.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <vector>

namespace Etf {

enum class BattleStates {
	NotInitialized,
	NotInBattle,
	BattleStartTriggered,
	Battle,
	BattleVictory,
	BattleSpoils,
	BattleEnd,
	BattleGameOver,
};
using enum BattleStates;

namespace BattleSystem {
void TriggerBattleStart();
void TriggerBattleVictoryStart();
void TriggerBattleSpoils();
void TriggerBattleEnd();
void TriggerGameOver();
void ResetAfterGameOver();
void BattleSystemUpdate();
void SendBattleDamage(int battlerNum, int damage);
void InitializeBattleSystem();
const std::vector<Battler*>& GetEnemyBattlers();
const AbilityData& GetAbilityByID(int id);
bool HasAbility(int id);
};	// namespace BattleSystem
}  // namespace Etf
