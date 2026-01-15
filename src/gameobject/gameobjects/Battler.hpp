#pragma once
#include <battle/battlerData.hpp>
#include <gameobject/GameObject.hpp>
struct Sprite;
namespace Etf {
class Battler : public GameObject {
   public:
	Battler(BattlerData* battleData, int x, int y);
	~Battler();
	void Draw() override;
	void StartAnimation(const std::string& name);

   private:
	Sprite* _sprite;
	unsigned int _animator;
	int _locationX, _locationY;
	BattlerData* _battlerData;
};

}  // namespace Etf
