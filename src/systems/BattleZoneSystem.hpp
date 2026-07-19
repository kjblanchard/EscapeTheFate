
#pragma once

#include "battle/battleZones.hpp"
namespace Etf {
class BattleZone;
namespace BattleZoneSystem {
void Start();
void Update();
void Draw();
void Shutdown();
void AddBattleZone(BattleZone* zone);
void DestroyBattleZoneGameObject(const BattleZone* zone);
const BattleZoneData& GetBattleZoneData(int id);
}  // namespace BattleZoneSystem

}  // namespace Etf
