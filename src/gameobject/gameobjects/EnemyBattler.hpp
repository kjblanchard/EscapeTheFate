#pragma once
#include <gameobject/gameobjects/Battler.hpp>
#include <vector>
struct Shader;
namespace Etf {
class UIProgressBar;
class UIAnimation;

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
	Shader* _blackoutShader = nullptr;
	float _deathEffectTime = 0.0f;
	bool _deathEffectPlaying = false;
	static constexpr float kDeathEffectDuration = 0.7f;

	UIAnimation* _atbBarAnim = nullptr;
	UIProgressBar* _atbProgressBar = nullptr;

	EnemyBattlerStates _enemyState = EnemyBattlerStates::ATBCharging;
	float _attackDelay = 0.0f;
	float _attackDelayTimer = 0.0f;
	float _blinkTimer = 0.0f;
	float _blinkToggleTimer = 0.0f;
	bool _blinkDark = false;
	static constexpr float kBlinkDuration = 0.4f;
	static constexpr float kBlinkToggleInterval = 0.06f;
};

}  // namespace Etf
