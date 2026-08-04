#pragma once
#include <battle/DamageNumberPool.hpp>
#include <battle/HitAnimPool.hpp>
#include <battle/battlerData.hpp>
#include <battle/battlerUI.hpp>
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
	inline float SpriteX() { return X() + _battlerData->Location.x; }
	float SpriteWidth();
	float SpriteHeight();
	inline float SpriteY() { return Y() + _battlerData->Location.y; }
	void Draw() override;
	inline void Update() override final {
		updateATBGauge();
		updateHitAnims();
		updateDamageNumbers();
		updateImpl();
	}

   public:
	inline unsigned int CurrentHP() { return _currentHP; }
	inline int CurrentAP() const { return _currentAP; }
	inline int MaxAP() const { return _maxAP; }
	inline int Def() { return _battlerData->Def; }
	inline const std::string& Name() const { return _battlerData->Name; }
	inline BattlerData* GetBattlerData() const { return _battlerData; }
	virtual bool IsPlayer() = 0;
	void PlayHitAnimation(const AbilityData& ability);

   public:
	void TakeDamage(int damage);
	void Heal(int amount);
#ifdef imgui
	inline int& DebugHP() { return _currentHP; }
	inline float& DebugATBCharge() { return _currentATBCharge; }
	inline int DebugMaxATB() { return _maxATBCharge; }
	inline int& DebugAP() { return _currentAP; }
	inline int DebugMaxAP() { return _maxAP; }
	inline BattlerData* DebugData() { return _battlerData; }
#endif

   protected:
	virtual void updateImpl() {}
	virtual void takeDamageImpl(int damage) = 0;
	virtual void healImpl(int amount) {}
	virtual void onAPGained() {}
	bool SpendAP(int cost);

   protected:
	BattlerData* _battlerData;
	int _currentHP;
	float _currentATBCharge;
	int _maxATBCharge;
	int _currentAP = 0;
	int _maxAP = 3;

   protected:
	Sprite* _sprite;
	int _locationX, _locationY;
	std::unique_ptr<SpriteAnimator> _animator;
	std::unordered_map<std::string, std::unique_ptr<HitAnimPool>> _hitAnimPools;
	DamageNumberPool _damageNumberPool;

   private:
	static constexpr const char* kDamageFont = "PressStart2P";
	static constexpr unsigned int kDamageFontSize = 8;
	void updateATBGauge();
	void updateHitAnims();
	void updateDamageNumbers();
};

}  // namespace Etf
