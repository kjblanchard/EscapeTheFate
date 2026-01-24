#pragma once
#include <battle/battlerUI.hpp>
#include <gameobject/GameObject.hpp>
#include <battle/battlerData.hpp>
struct Sprite;
namespace Etf {
class UIText;

struct BattlerArgs {
	BattlerData* BattleData;
	int X, Y, CurrentHP, BattlerNum;
    BattlerUI* BattleUI;
};
class Battler : public GameObject {
   public:
	Battler(const BattlerArgs& args);
	~Battler();
	void Draw() override;
	void StartAnimation(const std::string& name, bool backToIdle = true);

	inline unsigned int CurrentHP() { return _battlerData->HP; }
	void TakeDamage(int damage);

	// battle stat stuff
   private:
	// Stats loaded from db
	BattlerData* _battlerData;
	int _currentHP;

	// battle UI stuff
   private:
    BattlerUI* _battlerUI;

	// battle display and location stuff
   private:
	Sprite* _sprite;
	unsigned int _animator;
	int _locationX, _locationY;
	bool _isPlayer;
};

}  // namespace Etf
