#pragma once
#include <battle/battlerUI.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <memory>
namespace Etf {

class PlayerBattler : public Battler {
   public:
	PlayerBattler(const BattlerArgs& args);
	void updateImpl() override;

   protected:
	void updateUI() override;
	void takeDamageImpl(int damage) override;

   private:
	void handleInput();

   private:
	std::unique_ptr<BattlerUI> _battlerUI;
	unsigned int _currentMenuLocation = 0;
	void handleClickAction();
};

}  // namespace Etf
