#include <Supergoon/Input/keyboard.h>
#include <Supergoon/log.h>

#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameobject/gameobjects/PlayerBattler.hpp>
#include <systems/battleSystem.hpp>

using namespace Etf;
using namespace std;
using enum BattlerStates;

PlayerBattler::PlayerBattler(const BattlerArgs& args) : Battler(args), _battlerUI(make_unique<BattlerUI>(args.BattlerNum)) {
	_battlerUI->UpdateHP(to_string(_currentHP));
}

void PlayerBattler::handleStateChange(BattlerStates newState) {
	switch (newState) {
		case BattlerStates::ATBCharging:
			break;
		case BattlerStates::ATBFullyCharged:
			_battlerUI->OpenCommandsMenu();
			Engine::PlaySFX("playerTurn", 5.0f);
			break;
		default:
			break;
	}
	_currentBattlerState = newState;
}

void PlayerBattler::updateImpl() {
	_battlerUI->UpdateAnimations();
	switch (_currentBattlerState) {
		case BattlerStates::Default:
			handleStateChange(ATBCharging);
			break;
		case BattlerStates::ATBCharging: {
			auto progress = _currentATBCharge / _maxATBCharge * 100.00f;
			_battlerUI->UpdateProgressBar(progress);
			if (_currentATBCharge >= _maxATBCharge) {
				handleStateChange(ATBFullyCharged);
			}
			break;
		}
		case ATBFullyCharged: {
			handleStateChange(CommandSelection);
			break;
		}
		default:
			break;
	}
	handleInput();
}

void PlayerBattler::takeDamageImpl(int damage) {}

void PlayerBattler::handleInputCommandsMenu() {
	auto newLocation = _currentMenuLocation;
	if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.UP)) {
		--newLocation;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.DOWN)) {
		++newLocation;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.A)) {
		switch (_currentMenuLocation) {
			// Attack pressed, do the thing and back out of handling input here.
			case 0:
				Engine::PlaySFX("menuSelect", 1.0f);
				handleStateChange(TargetSelection);
				return;
			default:
				sgLogDebug("Button not implemented", _currentMenuLocation);
				Engine::PlaySFX("error1", 1.0f);
				break;
		}
	}

	if (newLocation != _currentMenuLocation) {
		_currentMenuLocation = newLocation > 3 ? _currentMenuLocation == 3 ? 0 : 3 : newLocation;
		_battlerUI->MoveCursorInMenu(_currentMenuLocation);
		Engine::PlaySFX("menuMove", 1.0f);
	}
}
void PlayerBattler::handleInputTargetSelection() {
	if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.UP)) {
		sgLogDebug("Move up one target");
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.DOWN)) {
		sgLogDebug("Move down one target");
	}
	// StartAnimation("slash2", 1);
	// BattleSystem::SendBattleDamage(4, 1);
	// _currentATBCharge = 0;
	// _battlerUI->CloseCommandsMenu();
	// handleStateChange(ATBCharging);
}

void PlayerBattler::handleInput() {
	switch (_currentBattlerState) {
		case BattlerStates::Default:
			break;
		case BattlerStates::ATBCharging:
			break;
		case BattlerStates::ATBFullyCharged:
			break;
		case BattlerStates::CommandSelection:
			handleInputCommandsMenu();
			break;
		case BattlerStates::TargetSelection:
			handleInputTargetSelection();
			break;
	}
}
