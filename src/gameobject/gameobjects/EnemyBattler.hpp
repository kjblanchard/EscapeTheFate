#pragma once
#include <gameobject/gameobjects/Battler.hpp>
namespace Etf {

class EnemyBattler : public Battler {
   public:
	EnemyBattler(const BattlerArgs& args);
	~EnemyBattler();
	void updateImpl() override;
	inline bool IsPlayer() override final { return false; };

   protected:
	void takeDamageImpl(int damage) override;

   private:
	UIText* _hpObject;
};

}  // namespace Etf
