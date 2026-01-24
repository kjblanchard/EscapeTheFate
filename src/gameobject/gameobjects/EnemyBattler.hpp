#pragma once
#include <gameobject/gameobjects/Battler.hpp>
namespace Etf {

class EnemyBattler : public Battler {
   public:
	EnemyBattler(const BattlerArgs& args);
	void updateImpl() override;

   protected:
	void updateUI() override;
	void takeDamageImpl(int damage) override;

   private:
	UIText* _hpObject;
};

}  // namespace Etf
