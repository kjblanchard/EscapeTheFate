#include <bindings/engine.hpp>
#include <Supergoon/log.h>
#include <gameobject/gameobjects/EnemyBattler.hpp>
#include <ui/ui.hpp>

using namespace Etf;
using namespace std;

EnemyBattler::EnemyBattler(const BattlerArgs& args) : Battler(args) {
	auto hpObject = UI::RootUIObject->GetChildByName("EnemyHP");
	_hpObject = static_cast<UIText*>(hpObject);
	_hpObject->UpdateText(to_string(_currentHP));
}

EnemyBattler::~EnemyBattler() {
	sgLogWarn("Destroying battler");
	if (_hpObject) {
		_hpObject->SetVisible(false);
	}
}

void EnemyBattler::updateImpl() {}
void EnemyBattler::takeDamageImpl(int damage) {
	_hpObject->UpdateText(to_string(_currentHP));
	if (_currentHP < 1) {
		Engine::PlaySFX("enemyDead", 1.0);
		Engine::SetSpriteVisible(_sprite, false);
	}
}
