#pragma once
#include <battle/battlerUI.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <memory>
#include <vector>
struct Shader;
namespace Etf {

enum class EnemyBattlerStates {
	ATBCharging,
	DelayBeforeAttack,
	Blinking,
	Attacking,
};

class EnemyBattler : public Battler {
   public:
	EnemyBattler(const BattlerArgs& args);
	~EnemyBattler();
	void updateImpl() override;
	inline bool IsPlayer() override final { return false; };

   protected:
	void takeDamageImpl(int damage) override;

   private:
	void getPlayerBattlers(std::vector<Battler*>& out);

   private:
	UIText* _hpObject;
	Shader* _deathShader = nullptr;
	float _deathEffectTime = 0.0f;
	bool _deathEffectPlaying = false;
	static constexpr float kDeathEffectDuration = 0.7f;

	std::unique_ptr<BattlerUI> _battlerUI;
	EnemyBattlerStates _enemyState = EnemyBattlerStates::ATBCharging;
	float _attackDelay = 0.0f;
	float _attackDelayTimer = 0.0f;
	float _blinkTimer = 0.0f;
	float _blinkToggleTimer = 0.0f;
	bool _blinkVisible = true;
	static constexpr float kBlinkDuration = 0.5f;
	static constexpr float kBlinkToggleInterval = 0.1f;
};

}  // namespace Etf
