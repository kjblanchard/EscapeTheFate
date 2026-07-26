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
	float _deathEffectTime = 0.0f;
	bool _deathEffectPlaying = false;
	static constexpr float kDeathEffectDuration = 0.7f;

	UIAnimation* _atbBarAnim = nullptr;
	UIProgressBar* _atbProgressBar = nullptr;
	UIProgressBar* _hpProgressBar = nullptr;

	EnemyBattlerStates _enemyState = EnemyBattlerStates::ATBCharging;
	float _attackDelay = 0.0f;
	float _attackDelayTimer = 0.0f;
	float _blinkToggleTimer = 0.0f;
	bool _blinkDark = false;
	int _blinkCount = 0;
	static constexpr int kBlinkTotal = 2;
	static constexpr float kBlinkToggleInterval = 0.1f;
};

}  // namespace Etf
