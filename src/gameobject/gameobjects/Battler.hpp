#pragma once
#include <battle/battlerData.hpp>
#include <battle/battlerUI.hpp>
#include <gameobject/GameObject.hpp>
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
	virtual ~Battler();
	inline float SpriteX() { return X() + _battlerData->Location.x; }
	inline float SpriteY() { return Y() + _battlerData->Location.y; }
	void Draw() override;
	// Updates the speed, and also calls updateimpl
	inline void Update() override final {
		updateATBGauge();
		updateImpl();
	}
	void StartAnimation(const std::string& name, bool backToIdle = true);

   public:
	inline unsigned int CurrentHP() { return _battlerData->HP; }
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
	unsigned int _animator;
	int _locationX, _locationY;

   private:
	void updateATBGauge();
};

}  // namespace Etf
