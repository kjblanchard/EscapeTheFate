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
		case BattlerStates::MagicSelection:
			_magicMenuRow = 0;
			_magicMenuCol = 0;
			_battlerUI->OpenMagicMenu();
			break;
		case BattlerStates::TargetSelection: {
			_currentTargetBattler = 0;
			_battlerUI->StartTargetSelection();
			moveFingerToTargetNum(0);
			break;
		}
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
	if (_isDead) {
		if (_deathHoldTimer > 0) {
			_deathHoldTimer -= GameState::DeltaTimeSeconds;
			if (_deathHoldTimer <= 0) {
				_animator->UpdateAnimatorSpeed(0.0f);
			}
		}
		return;
	}
	if (_currentHP <= 0) return;
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
	if (_currentHP <= 0) {
		Engine::Audio::PlaySFXBuffer("hit2", 1.0f);
		_animator->StartAnimation("dead1", -1);
		_isDead = true;
		_deathHoldTimer = _animator->GetAnimationDuration("dead1");
		_isPlayingDamageAnim = false;
		_battlerUI->CloseCommandsMenu();
		_battlerUI->CloseTargetSelection();
		_battlerUI->EndPlayerTurn(this);
		return;
	}
	if (!_isPlayingDamageAnim) {
		_isPlayingDamageAnim = true;
		_damageAnimTimer = 0.5f;
		_animator->PlayAnimationThenLoopSecond("damage1", _battlerData->IdleAnimation);
	}
}

void PlayerBattler::healImpl(int amount) {
	_battlerUI->UpdateHP(to_string(_currentHP));
}

void PlayerBattler::handleInputCommandsMenu() {
	auto newLocation = _currentMenuLocation;
	if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.UP)) {
		--newLocation;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.DOWN)) {
		++newLocation;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.A)) {
		switch (_currentMenuLocation) {
			case 0:
				Engine::Audio::PlaySFXBuffer("menuSelect", 1.0f);
				_selectedAbilityID = 0;
				_targetingFriendly = false;
				handleStateChange(TargetSelection);
				return;
			case 1:
				Engine::Audio::PlaySFXBuffer("menuSelect", 1.0f);
				handleStateChange(MagicSelection);
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

void PlayerBattler::getPlayerBattlers(std::vector<Battler*>& battlerVector) {
	auto battlers = BattleSystem::GetEnemyBattlers();
	copy_if(battlers.begin(), battlers.end(), back_inserter(battlerVector), [](Battler* battler) {
		return battler && battler->IsPlayer() && battler->CurrentHP() > 0;
	});
}

void PlayerBattler::getAllTargets(std::vector<Battler*>& battlerVector) {
	if (_targetingFriendly) {
		getPlayerBattlers(battlerVector);
	} else {
		getEnemyBattlers(battlerVector);
	}
}

void PlayerBattler::moveFingerToTargetNum(int targetNum) {
	std::vector<Battler*> targets;
	getAllTargets(targets);
	if (targets.empty()) return;

	if (targetNum > (int)targets.size() - 1) {
		targetNum = 0;
	} else if (targetNum < 0) {
		targetNum = (int)targets.size() - 1;
	}
	const auto battler = targets.at(targetNum);
	if (battler) {
		_battlerUI->MoveFingerToBattlerLocation(battler);

		string displayName = battler->Name();
		int sameNameCount = 0;
		int ordinal = 0;
		for (size_t i = 0; i < targets.size(); ++i) {
			if (targets[i]->Name() == displayName) {
				if ((int)i < targetNum) ++ordinal;
				++sameNameCount;
			}
		}
		if (sameNameCount > 1) {
			displayName += ' ';
			displayName += ('A' + ordinal);
		}
		_battlerUI->UpdateTargetInfo(displayName);
	}
	_currentTargetBattler = targetNum;
}

void PlayerBattler::handleInputMagicMenu() {
	auto newRow = _magicMenuRow;
	auto newCol = _magicMenuCol;
	if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.UP)) {
		if (newRow > 0) --newRow;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.DOWN)) {
		if (newRow < 3) ++newRow;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.LEFT)) {
		if (newCol > 0) --newCol;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.RIGHT)) {
		if (newCol < 1) ++newCol;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.A)) {
		int slotIndex = _magicMenuCol * 4 + _magicMenuRow;
		_selectedAbilityID = slotIndex + 1;
		if (!BattleSystem::HasAbility(_selectedAbilityID)) {
			Engine::Audio::PlaySFXBuffer("error1", 1.0f);
			return;
		}
		Engine::Audio::PlaySFXBuffer("menuSelect", 1.0f);
		const auto& ability = BattleSystem::GetAbilityByID(_selectedAbilityID);
		_targetingFriendly = ability.Friendly;
		handleStateChange(TargetSelection);
		return;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.B)) {
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		_battlerUI->CloseMagicMenu();
		_currentBattlerState = CommandSelection;
		return;
	}

	if (newRow != _magicMenuRow || newCol != _magicMenuCol) {
		_magicMenuRow = newRow;
		_magicMenuCol = newCol;
		_battlerUI->MoveCursorInMagicMenu(_magicMenuCol, _magicMenuRow);
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
	}
}

void PlayerBattler::handleInputTargetSelection() {
	int newTarget = _currentTargetBattler;

	if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.UP)) {
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		--newTarget;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.DOWN)) {
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		++newTarget;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.LEFT) ||
			   IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.RIGHT)) {
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		_targetingFriendly = !_targetingFriendly;
		_currentTargetBattler = 0;
		moveFingerToTargetNum(0);
		return;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.A)) {
		Engine::Audio::PlaySFXBuffer("menuSelect", 1.0f);
		vector<Battler*> targets;
		getAllTargets(targets);
		if (targets.empty()) return;
		const auto battler = targets.at(_currentTargetBattler);
		const auto& ability = BattleSystem::GetAbilityByID(_selectedAbilityID);
		const auto& playerAnim = ability.PlayerAnim.empty() ? "slash2" : ability.PlayerAnim;
		_animator->PlayAnimationThenLoopSecond(playerAnim, _battlerData->IdleAnimation);
		if (battler) {
			if (ability.BaseDamage < 0) {
				battler->Heal(-ability.BaseDamage);
			} else {
				battler->TakeDamage(ability.BaseDamage);
			}
			battler->PlayHitAnimation(ability);
		}
		_currentATBCharge = 0;
		handleStateChange(ATBCharging);
		return;
	} else if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.B)) {
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		_battlerUI->CloseTargetSelection();
		if (_selectedAbilityID > 0) {
			_currentBattlerState = MagicSelection;
		} else {
			_currentBattlerState = CommandSelection;
		}
		return;
	}

	if (newTarget != _currentTargetBattler) {
		moveFingerToTargetNum(newTarget);
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
		case BattlerStates::MagicSelection:
			handleInputMagicMenu();
			break;
		case BattlerStates::TargetSelection:
			handleInputTargetSelection();
			break;
		case BattlerStates::BattleEndIdle:
			if (IsKeyboardKeyJustPressed(GameConfig::GetGameConfig().Controls.Keyboard.A)) {
				handleStateChange(BattlerStates::BattleSpoils);
				BattleSystem::TriggerBattleSpoils();
			}
			break;
		case BattlerStates::BattleSpoils:
			break;
		default:
			break;
	}
}
