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
	void Draw() override;
	virtual void Update() override = 0;
	void StartAnimation(const std::string& name, bool backToIdle = true);

   public:
	inline unsigned int CurrentHP() { return _battlerData->HP; }

   public:
	void TakeDamage(int damage);

   protected:
    //fully updates the ui
	virtual void updateUI() {}
    // what should happen when you take damage
	virtual void takeDamageImpl(int damage) = 0;

   protected:
	BattlerData* _battlerData;
	int _currentHP;

   protected:
	Sprite* _sprite;
	unsigned int _animator;
	int _locationX, _locationY;
};

}  // namespace Etf
