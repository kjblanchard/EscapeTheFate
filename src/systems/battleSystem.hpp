#pragma once
#include <battle/abilityData.hpp>
#include <cstdint>
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
const BattlerData* GetPlayerBattlerData(int playerIndex);
int GetBattlerSlotIndex(const Battler* battler);
void ApplyRemoteUIState(uint8_t battlerState, uint8_t menuCursor, uint8_t magicRow, uint8_t magicCol, uint8_t targetIndex, uint8_t targetingFriendly, uint8_t selectedAbilityID);
void ApplyEnemyAction(uint8_t enemySlot, uint8_t abilityID, uint8_t targetSlot);
};	// namespace BattleSystem
}  // namespace Etf
