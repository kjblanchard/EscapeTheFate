#pragma once
#include <gameobject/gameobjects/Battler.hpp>
#include <vector>

namespace Etf {

enum class BattleStates {
	NotInitialized,
	NotInBattle,
	BattleStartTriggered,
	Battle,
	BattleVictory,
	BattleEnd,
};
using enum BattleStates;

namespace BattleSystem {
// Triggers the battle system to initialize on the next frame
void TriggerBattleStart();
// Changes battle state to victory screen
void TriggerBattleVictoryStart();
void TriggerBattleEnd();
void BattleSystemUpdate();
void SendBattleDamage(int battlerNum, int damage);
void InitializeBattleSystem();
const std::vector<Battler*>& GetEnemyBattlers();
};	// namespace BattleSystem
}  // namespace Etf
