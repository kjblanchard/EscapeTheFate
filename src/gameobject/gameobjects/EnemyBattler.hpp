#pragma once
#include <gameobject/gameobjects/Battler.hpp>
struct Shader;
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
	Shader* _deathShader = nullptr;
	float _deathEffectTime = 0.0f;
	bool _deathEffectPlaying = false;
	static constexpr float kDeathEffectDuration = 1.0f;
};

}  // namespace Etf
