#include <gameobject/gameobjects/EnemyBattler.hpp>
#include <ui/ui.hpp>

using namespace Etf;
using namespace std;

EnemyBattler::EnemyBattler(const BattlerArgs& args) : Battler(args) {
	auto hpObject = UI::RootUIObject->GetChildByName("EnemyHP");
	_hpObject = static_cast<UIText*>(hpObject);
	_hpObject->UpdateText(to_string(_currentHP));
}

void EnemyBattler::updateImpl() {}
void EnemyBattler::takeDamageImpl(int damage) {
	_hpObject->UpdateText(to_string(_currentHP));
}
