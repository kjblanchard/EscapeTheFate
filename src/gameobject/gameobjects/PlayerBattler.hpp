#pragma once
#include <battle/battlerUI.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <memory>
namespace Etf {

enum class BattlerStates {
	Default,
	ATBCharging,
	ATBFullyCharged,
	CommandSelection,
	TargetSelection,
};

class PlayerBattler : public Battler {
   public:
	PlayerBattler(const BattlerArgs& args);
	inline bool IsPlayer() override final { return true; };

   private:
	void updateImpl() override;
	void takeDamageImpl(int damage) override;
	void handleInputCommandsMenu();
	void handleInputTargetSelection();
	// Handles input based on current state.
	void handleInput();
	// Handles the state changes of a battler
	void handleStateChange(BattlerStates newState);
	// What happens when something is clicked
	void handleClickAction();

   private:
	std::unique_ptr<BattlerUI> _battlerUI;
	unsigned int _currentMenuLocation = 0;
	unsigned int _currentTargetBattler;
	BattlerStates _currentBattlerState = BattlerStates::Default;
};

}  // namespace Etf
