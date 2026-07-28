#pragma once
#include <battle/battlerUI.hpp>
#include <components/PlayerController.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <memory>
namespace Etf {

enum class BattlerStates {
	Default,
	Initialized,
	ATBCharging,
	ATBFullyCharged,
	CommandSelection,
	MagicSelection,
	TargetSelection,
	BattleEndStart,
	BattleEndIdle,
	BattleSpoils,
	BattleEnd,
};

class PlayerBattler : public Battler {
   public:
	PlayerBattler(const BattlerArgs& args);
	inline bool IsPlayer() override final { return true; };

   private:
	void updateImpl() override;
	void takeDamageImpl(int damage) override;
	void healImpl(int amount) override;
	void handleInputCommandsMenu();
	void handleInputMagicMenu();
	void handleInputTargetSelection();
	void handleInput();
	void handleStateChange(BattlerStates newState);
	void handleClickAction();
	void getEnemyBattlers(std::vector<Battler*>& battlerVector);
	void getPlayerBattlers(std::vector<Battler*>& battlerVector);
	void getAllTargets(std::vector<Battler*>& battlerVector);
	void moveFingerToTargetNum(int targetNum);
	void moveFingerToEnemyNum(int enemyNum);
	bool shouldBattleEnd();
	void startATBAnimation();

#ifdef imgui
   public:
	inline BattlerStates& DebugState() { return _currentBattlerState; }
#endif

   private:
	std::shared_ptr<PlayerController> _controller;
	std::unique_ptr<BattlerUI> _battlerUI;
	unsigned int _currentMenuLocation = 0;
	unsigned int _magicMenuRow = 0;
	unsigned int _magicMenuCol = 0;
	int _currentTargetBattler = 0;
	int _selectedAbilityID = 0;
	bool _targetingFriendly = false;
	BattlerStates _currentBattlerState = BattlerStates::Default;
	float _damageAnimTimer = 0.0f;
	float _deathHoldTimer = 0.0f;
	bool _isPlayingDamageAnim = false;
	bool _isDead = false;
};

}  // namespace Etf
