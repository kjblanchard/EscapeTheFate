#pragma once
#include <gameobject/gameobjects/Battler.hpp>
#include <vector>

namespace Etf {

class BattleSystem {
   public:
	// Triggers the battle system to initialize on the next frame
	static void TriggerBattleStart();
	static void BattleSystemUpdate();
	static void SendBattleDamage(int battlerNum, int damage);

	static const std::vector<Battler*>& GetEnemyBattlers();
};
}  // namespace Etf
