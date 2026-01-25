#include <Supergoon/Input/keyboard.h>
#include <Supergoon/log.h>

#include <algorithm>
#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameobject/gameobjects/PlayerBattler.hpp>
#include <iterator>
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
			_battlerUI->CloseTargetSelection();
			break;
		case BattlerStates::ATBFullyCharged:
			_battlerUI->OpenCommandsMenu();
			Engine::PlaySFX("playerTurn", 5.0f);
			break;
		case BattlerStates::TargetSelection:
			_battlerUI->StartTargetSelection();
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
		case TargetSelection:
			break;
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
	static int _currentTarget = 0;
	auto battlers = BattleSystem::GetEnemyBattlers();
	std::vector<Battler*> enemyBattlers;
	copy_if(battlers.begin(), battlers.end(), back_inserter(enemyBattlers), [](Battler* battler) {
		return battler && !battler->IsPlayer();
	});

	if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.UP)) {
		_currentTarget = _currentTarget - 1 >= 0 ? _currentTarget - 1 : enemyBattlers.size() - 1;
		const auto battler = enemyBattlers.at(_currentTarget);
		if (battler) {
			_battlerUI->MoveFingerToBattlerLocation(battler);
		}
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.DOWN)) {
		_currentTarget = _currentTarget + 1 > enemyBattlers.size() - 1 ? 0 : _currentTarget + 1;
		const auto battler = enemyBattlers.at(_currentTarget);
		if (battler) {
			_battlerUI->MoveFingerToBattlerLocation(battler);
		}
	}
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
