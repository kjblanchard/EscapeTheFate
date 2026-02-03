#pragma once
#include <battle/battlerData.hpp>
#include <battle/battlerUI.hpp>
#include <bindings/SpriteAnimator.hpp>
#include <gameobject/GameObject.hpp>
#include <memory>
struct Sprite;
namespace Etf {
class UIText;

struct BattlerArgs {
	BattlerData* BattleData;
	int X, Y, CurrentHP, BattlerNum;
};

class Battler : public GameObject {
   public:
	Battler(const BattlerArgs& args);
	virtual ~Battler() = default;
	inline float SpriteX() { return X() + _battlerData->Location.x; }
	float SpriteWidth();
	float SpriteHeight();
	inline float SpriteY() { return Y() + _battlerData->Location.y; }
	void Draw() override;
	// Updates the speed, and also calls updateimpl
	inline void Update() override final {
		updateATBGauge();
		updateImpl();
	}

   public:
	inline unsigned int CurrentHP() { return _currentHP; }
	virtual bool IsPlayer() = 0;

   public:
	void TakeDamage(int damage);

   protected:
	// called by update each frame
	virtual void updateImpl() {}
	// what should happen when you take damage
	virtual void takeDamageImpl(int damage) = 0;

   protected:
	BattlerData* _battlerData;
	int _currentHP;
	float _currentATBCharge;
	int _maxATBCharge;

   protected:
	Sprite* _sprite;
	int _locationX, _locationY;
	std::unique_ptr<SpriteAnimator> _animator;

   private:
	void updateATBGauge();
};

}  // namespace Etf
