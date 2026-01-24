#include <Supergoon/Input/keyboard.h>
#include <Supergoon/log.h>

#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameobject/gameobjects/PlayerBattler.hpp>
#include <systems/battleSystem.hpp>

using namespace Etf;
using namespace std;

PlayerBattler::PlayerBattler(const BattlerArgs& args) : Battler(args), _battlerUI(make_unique<BattlerUI>(args.BattlerNum)) {
	_battlerUI->UpdateHP(to_string(_currentHP));
}
void PlayerBattler::updateImpl() {
	_battlerUI->UpdateAnimations();
	if (_currentATBCharge < _maxATBCharge) {
		auto progress = _currentATBCharge / _maxATBCharge * 100.00f;
		_battlerUI->UpdateProgressBar(progress);
		return;
	}
	if (!_turnStarted && _currentATBCharge >= _maxATBCharge) {
		sgLogWarn("Starting player turn!");
		_turnStarted = true;
		_battlerUI->OpenCommandsMenu();
	}
	handleInput();
}
void PlayerBattler::updateUI() {
}
void PlayerBattler::takeDamageImpl(int damage) {}
void PlayerBattler::handleInput() {
	if (_currentATBCharge < _maxATBCharge) return;
	auto newLocation = _currentMenuLocation;
	if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.UP)) {
		--newLocation;
	}

	else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.DOWN)) {
		++newLocation;
	}
	if (newLocation != _currentMenuLocation) {
		_currentMenuLocation = newLocation > 3 ? _currentMenuLocation == 3 ? 0 : 3 : newLocation;
		_battlerUI->MoveCursorInMenu(_currentMenuLocation);
		Engine::PlaySFX("menuMove", 1.0f);
	}

	else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.A)) {
		handleClickAction();
	}
}

void PlayerBattler::handleClickAction() {
	sgLogDebug("Just pressed button num %d", _currentMenuLocation);
	switch (_currentMenuLocation) {
		case 0:
			Engine::PlaySFX("menuSelect", 1.0f);
			StartAnimation("slash2", 1);
			BattleSystem::SendBattleDamage(4, 1);
			_currentATBCharge = 0;
			_battlerUI->CloseCommandsMenu();
			_turnStarted = false;
			break;
		default:
			sgLogDebug("Button not implemented", _currentMenuLocation);
			Engine::PlaySFX("error1", 1.0f);
			break;
	}
}
