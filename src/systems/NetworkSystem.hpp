#pragma once
#include <cstdint>

namespace Etf {
namespace NetworkSystem {
void Start();
void Update();
void Shutdown();

void Connect();
bool IsConnected();
bool IsConnecting();
bool ConnectionFailed();

void SendMove(float x, float y, uint8_t direction, bool moving);
void SendInteract(float x, float y);
void SendBattleAction(uint8_t battlerNum, int16_t damage);
void SendJoin(uint8_t characterIndex);
void SendSceneChange(const char* mapName, uint8_t loadLocation);
void SendBattleStart(uint8_t battleGroup, const char* battleScene);
void SendBattleUIState(uint8_t battlerState, uint8_t menuCursor, uint8_t magicRow, uint8_t magicCol, uint8_t targetIndex, uint8_t targetingFriendly, uint8_t selectedAbilityID);
void SendBattleEnemyAction(uint8_t enemySlot, uint8_t abilityID, uint8_t targetSlot);
}  // namespace NetworkSystem
}  // namespace Etf
