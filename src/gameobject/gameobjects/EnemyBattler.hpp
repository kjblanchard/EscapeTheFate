#pragma once
#include <battle/enemyAI.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <memory>
#include <vector>
struct Shader;
namespace Etf {
class UIPanel;
class UIProgressBar;

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
	void healImpl(int amount) override;
	void onAPGained() override;

   private:
	void getPlayerBattlers(std::vector<Battler*>& out);
	void getEnemyAllies(std::vector<Battler*>& out);

   public:
	void TriggerRemoteAction(int abilityID, int targetSlot);

   private:
	std::unique_ptr<EnemyAIStrategy> _ai;
	AIAction _pendingAction;
	Shader* _deathShader = nullptr;
	float _deathEffectTime = 0.0f;
	bool _deathEffectPlaying = false;
	static constexpr float kDeathEffectDuration = 0.7f;

	UIPanel* _barPanel = nullptr;
	UIProgressBar* _atbProgressBar = nullptr;
	UIProgressBar* _hpProgressBar = nullptr;

	EnemyBattlerStates _enemyState = EnemyBattlerStates::ATBCharging;
	float _attackDelay = 0.0f;
	float _attackDelayTimer = 0.0f;
	float _blinkToggleTimer = 0.0f;
	bool _blinkDark = false;
	int _blinkCount = 0;
	static constexpr int kBlinkTotal = 2;
	static constexpr float kBlinkDarkTime = 0.06f;
	static constexpr float kBlinkLightTime = 0.12f;
};

}  // namespace Etf
