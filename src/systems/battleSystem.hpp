#pragma once

namespace Etf {

class BattleSystem {
   public:
	// Triggers the battle system to initialize on the next frame
	static void TriggerBattleStart();
	static void BattleSystemUpdate();
	static void SendBattleDamage(int battlerNum, int damage);
};
}  // namespace Etf
