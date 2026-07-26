#include <Supergoon/Input/keyboard.h>
#include <sgtools/log.h>

#include <algorithm>
#include <engine.hpp>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/PlayerBattler.hpp>
#include <iterator>
#include <systems/battleSystem.hpp>

using namespace Etf;
using namespace std;
using enum BattlerStates;

const string VICTORY_STR = "cheer1";

bool PlayerBattler::shouldBattleEnd() {
	std::vector<Battler*> enemyBattlers;
	getEnemyBattlers(enemyBattlers);
	if (enemyBattlers.size() < 1) {
		return true;
	}
	return false;
}

PlayerBattler::PlayerBattler(const BattlerArgs& args) : Battler(args), _battlerUI(make_unique<BattlerUI>(args.BattlerNum)) {
	_battlerUI->UpdateHP(to_string(_currentHP));
}

void PlayerBattler::handleStateChange(BattlerStates newState) {
	if ((newState == ATBCharging || newState == ATBFullyCharged || newState == TargetSelection) && shouldBattleEnd()) newState = BattleEndStart;
	switch (newState) {
		case BattlerStates::ATBCharging:
			_battlerUI->StartATBIdleAnim();
			_battlerUI->CloseCommandsMenu();
			_battlerUI->CloseTargetSelection();
			_battlerUI->EndPlayerTurn(this);
			break;
		case BattlerStates::ATBFullyCharged:
			_battlerUI->StartATBTurnAnim();
			_battlerUI->OpenCommandsMenu();
			Engine::Audio::PlaySFXBuffer("playerTurn", 5.0f);
			_battlerUI->StartPlayerTurn(this);
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
			_animator->AddAnimationToQueue(VICTORY_STR, true);
			_battlerUI->EndPlayerTurn(this);
			BattleSystem::TriggerBattleVictoryStart();
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
	sgLogDebug("Trying to move finger to location %d", enemyNum);
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
	sgLogDebug("Trying to move to location bounds %d", enemyNum);
	const auto battler = enemyBattlers.at(enemyNum);
	if (battler) {
		_battlerUI->MoveFingerToBattlerLocation(battler);

		string displayName = battler->Name();
		int sameNameCount = 0;
		int ordinal = 0;
		for (size_t i = 0; i < enemyBattlers.size(); ++i) {
			if (enemyBattlers[i]->Name() == displayName) {
				if ((int)i < enemyNum) ++ordinal;
				++sameNameCount;
			}
		}
		if (sameNameCount > 1) {
			displayName += ' ';
			displayName += ('A' + ordinal);
		}
		_battlerUI->UpdateTargetInfo(displayName);
	}
	_currentTargetBattler = enemyNum;
}

void PlayerBattler::updateImpl() {
	_battlerUI->UpdateAnimations();
	if (_isPlayingDamageAnim) {
		_damageAnimTimer -= GameState::DeltaTimeSeconds;
		if (_damageAnimTimer <= 0) {
			_isPlayingDamageAnim = false;
		}
	}
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

void PlayerBattler::takeDamageImpl(int damage) {
	_battlerUI->UpdateHP(to_string(_currentHP));
	if (!_isPlayingDamageAnim) {
		_isPlayingDamageAnim = true;
		_damageAnimTimer = 0.5f;
		_animator->PlayAnimationThenLoopSecond("damage1", _battlerData->IdleAnimation);
	}
}

void PlayerBattler::handleInputCommandsMenu() {
	auto newLocation = _currentMenuLocation;
	if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.UP)) {
		--newLocation;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.DOWN)) {
		++newLocation;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.A)) {
		switch (_currentMenuLocation) {
			// Attack pressed, do the thing and back out of handling input here.
			case 0:
				Engine::Audio::PlaySFXBuffer("menuSelect", 1.0f);
				handleStateChange(TargetSelection);
				return;
			default:
				sgLogDebug("Button not implemented", _currentMenuLocation);
				Engine::Audio::PlaySFXBuffer("error1", 1.0f);
				break;
		}
	}

	if (newLocation != _currentMenuLocation) {
		_currentMenuLocation = newLocation > 3 ? _currentMenuLocation == 3 ? 0 : 3 : newLocation;
		_battlerUI->MoveCursorInMenu(_currentMenuLocation);
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
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

	if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.UP)) {
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		--newTarget;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.DOWN)) {
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		++newTarget;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.A)) {
		Engine::Audio::PlaySFXBuffer("menuSelect", 1.0f);
		vector<Battler*> battlers;
		getEnemyBattlers(battlers);
		const auto battler = battlers.at(newTarget);
		_animator->PlayAnimationThenLoopSecond("slash2", _battlerData->IdleAnimation);
		if (battler) {
			const auto& ability = BattleSystem::GetAbilityByID(0);
			battler->TakeDamage(ability.BaseDamage);
			battler->PlayHitAnimation(ability);
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
			if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.A)) {
				handleStateChange(BattlerStates::BattleEnd);
			}
			break;
		default:
			break;
	}
}
