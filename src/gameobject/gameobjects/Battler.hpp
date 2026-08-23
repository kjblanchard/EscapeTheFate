#pragma once
#include <battle/DamageNumberPool.hpp>
#include <battle/HitAnimPool.hpp>
#include <battle/battlerData.hpp>
#include <battle/battlerUI.hpp>
#include <battle/statuseffects/statusEffects.hpp>
#include <components/SpriteAnimator.hpp>
#include <gameobject/GameObject.hpp>
#include <memory>
#include <string>
#include <unordered_map>
struct Sprite;
namespace Etf {
struct AbilityData;
class UIText;
class PlayerController;

struct BattlerArgs {
	BattlerData* BattleData;
	int X, Y, CurrentHP, BattlerNum;
	std::shared_ptr<PlayerController> Controller;
};

class Battler : public GameObject {
   public:
	Battler(const BattlerArgs& args);
	~Battler() override;
	inline float SpriteX() { return X() + battlerData->Location.x; }
	float SpriteWidth();
	float SpriteHeight();
	inline float SpriteY() { return Y() + battlerData->Location.y; }
	void draw() override;
	inline void update() override final {
		updateATBGauge();
		updateHitAnims();
		updateDamageNumbers();
		updateImpl();
	}

   public:
	inline unsigned int CurrentHP() { return currentHP; }
	inline int CurrentAP() const { return currentAP; }
	inline int MaxAP() const { return maxAP; }
	inline int Def() { return battlerData->Def; }
	inline const std::string& Name() const { return battlerData->Name; }
	inline BattlerData* GetBattlerData() const { return battlerData; }
	inline void ApplyStatusEffect(StatusEffectInstance& s) { statusEffects.push_back(s); }
	virtual bool IsPlayer() = 0;
	void PlayHitAnimation(const AbilityData& ability);
	int GetOutgoingDamageBonus() const;
	void AddSpdBonus(int amount);

   public:
	void TakeDamage(int damage);
	void Heal(int amount);
#ifdef imgui
	inline int& DebugHP() { return currentHP; }
	inline float& DebugATBCharge() { return currentATBCharge; }
	inline int DebugMaxATB() { return maxATBCharge; }
	inline int& DebugAP() { return currentAP; }
	inline int DebugMaxAP() { return maxAP; }
	inline BattlerData* DebugData() { return battlerData; }
#endif

   protected:
	virtual void updateImpl() {}
	virtual void takeDamageImpl(int damage) = 0;
	virtual void healImpl(int amount) {}
	virtual void onAPGained() {}
	void handleTurnStartStatus();
	void handleTurnEndStatus();
	void handleShouldApplyStatus(Battler* b);
	int absorbDamage(int rawDamage);
	bool spendAP(int cost);

   protected:
	BattlerData* battlerData;
	int currentHP;
	float currentATBCharge;
	int maxATBCharge;
	int currentAP = 0;
	int maxAP = 3;
	int spdBonus = 0;

   protected:
	Sprite* sprite;
	int locationX, locationY;
	std::unique_ptr<SpriteAnimator> animator;
	std::unordered_map<std::string, std::unique_ptr<HitAnimPool>> hitAnimPools;
	DamageNumberPool damageNumberPool;
	std::vector<StatusEffectInstance> statusEffects;

   private:
	static constexpr const char* kDamageFont = "PressStart2P";
	static constexpr unsigned int kDamageFontSize = 8;
	void updateATBGauge();
	void updateHitAnims();
	void updateDamageNumbers();
};

}  // namespace Etf
