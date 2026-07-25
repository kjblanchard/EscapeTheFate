#pragma once
#include <battle/enemyBattlerUI.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <memory>
struct Shader;
namespace Etf {

enum class EnemyBattlerStates {
	Default,
	ATBCharging,
	ATBFullyCharged,
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
	std::unique_ptr<EnemyBattlerUI> _enemyUI;
	Shader* _deathShader = nullptr;
	float _deathEffectTime = 0.0f;
	bool _deathEffectPlaying = false;
	EnemyBattlerStates _currentState = EnemyBattlerStates::Default;
	static constexpr float kDeathEffectDuration = 0.7f;
};

}  // namespace Etf
