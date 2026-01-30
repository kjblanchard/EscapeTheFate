#include <Supergoon/Input/keyboard.h>
#include <Supergoon/log.h>
#include <gameState.hpp>

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
	if (newState == ATBCharging || newState == ATBFullyCharged || newState == TargetSelection) {
		std::vector<Battler*> enemyBattlers;
		getEnemyBattlers(enemyBattlers);
		if (enemyBattlers.size() < 1) {
			newState = BattleEndStart;
		}
	}
	switch (newState) {
		case BattlerStates::ATBCharging:
			_battlerUI->CloseCommandsMenu();
			_battlerUI->CloseTargetSelection();
			break;
		case BattlerStates::ATBFullyCharged:
			_battlerUI->OpenCommandsMenu();
			Engine::PlaySFX("playerTurn", 5.0f);
			break;
		case BattlerStates::TargetSelection:
			_currentTargetBattler = 0;
			_battlerUI->StartTargetSelection();
			moveFingerToEnemyNum(0);
			break;
		case BattlerStates::BattleEndStart:
			Engine::Audio::PlayBGM("victory");
			_battlerUI->CloseCommandsMenu();
			_battlerUI->CloseTargetSelection();
			_battlerUI->ClosePlayerInfoBox();
			StartAnimation("cheer1", false);
			break;
		case BattlerStates::BattleEnd:
			BattleSystem::TriggerBattleEnd();
			break;
		default:
			break;
	}
	_currentBattlerState = newState;
}
void PlayerBattler::moveFingerToEnemyNum(int enemyNum) {
	sgLogDebug("Trying to move to location %d", enemyNum);
	std::vector<Battler*> enemyBattlers;
	getEnemyBattlers(enemyBattlers);
	if (enemyBattlers.empty()) {
		return;
	}

	if (enemyNum > (int)enemyBattlers.size() - 1) {
		enemyNum = 0;
	} else if (enemyNum < 0) {
		enemyNum = (int)enemyBattlers.size() - 1;
	}
	sgLogWarn("Trying to move to location bounds %d", enemyNum);
	const auto battler = enemyBattlers.at(enemyNum);
	if (battler) {
		_battlerUI->MoveFingerToBattlerLocation(battler);
	}
	_currentTargetBattler = enemyNum;
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
		case BattleEndStart:
			handleStateChange(BattleEndIdle);
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
void PlayerBattler::getEnemyBattlers(std::vector<Battler*>& battlerVector) {
	auto battlers = BattleSystem::GetEnemyBattlers();
	copy_if(battlers.begin(), battlers.end(), back_inserter(battlerVector), [](Battler* battler) {
		return battler && !battler->IsPlayer() && battler->CurrentHP() > 0;
	});
}

void PlayerBattler::handleInputTargetSelection() {
	int newTarget = _currentTargetBattler;

	if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.UP)) {
		Engine::PlaySFX("menuMove", 1.0f);
		--newTarget;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.DOWN)) {
		Engine::PlaySFX("menuMove", 1.0f);
		++newTarget;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.A)) {
		Engine::PlaySFX("menuSelect", 1.0f);
		vector<Battler*> battlers;
		getEnemyBattlers(battlers);
		const auto battler = battlers.at(newTarget);
		StartAnimation("slash2");
		if (battler) {
			battler->TakeDamage(1);
		}
		_currentATBCharge = 0;
		handleStateChange(ATBCharging);
	}

	if (newTarget != _currentTargetBattler) {
		moveFingerToEnemyNum(newTarget);
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
		case BattlerStates::BattleEndIdle:
			if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.A)) {
				handleStateChange(BattleEnd);
			}
			break;
	}
}
