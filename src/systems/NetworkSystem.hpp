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
}  // namespace NetworkSystem
}  // namespace Etf
