#pragma once
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
		updateImpl();
	}

   public:
	inline unsigned int CurrentHP() { return _currentHP; }
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
	inline BattlerData* DebugData() { return _battlerData; }
#endif

   protected:
	virtual void updateImpl() {}
	virtual void takeDamageImpl(int damage) = 0;
	virtual void healImpl(int amount) {}

   protected:
	BattlerData* _battlerData;
	int _currentHP;
	float _currentATBCharge;
	int _maxATBCharge;

   protected:
	Sprite* _sprite;
	int _locationX, _locationY;
	std::unique_ptr<SpriteAnimator> _animator;
	std::unordered_map<std::string, std::unique_ptr<HitAnimPool>> _hitAnimPools;

   private:
	void updateATBGauge();
	void updateHitAnims();
};

}  // namespace Etf
