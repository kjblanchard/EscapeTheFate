#include <Supergoon/camera.h>
#include <sgtools/log.h>

#include <algorithm>
#include <components/PlayerController.hpp>
#include <engine.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/PlayerBattler.hpp>
#include <iterator>
#include <systems/PointerInputSystem.hpp>
#include <systems/battleSystem.hpp>
#include <types/ControllerButtons.hpp>

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

PlayerBattler::PlayerBattler(const BattlerArgs& args) : Battler(args), _controller(args.Controller), _battlerUI(make_unique<BattlerUI>(args.BattlerNum)) {
	_battlerUI->UpdateHP(to_string(currentHP));
	_battlerUI->UpdateAP(to_string(currentAP));
	_battlerUI->UpdateName(battlerData->Nick);
	int i = 0;
	for (auto a : battlerData->Abilities) {
		auto& ad = BattleSystem::GetAbilityByID(a);
		auto n = ad.Name;
		_battlerUI->UpdateMagicMenu(i, ad.Name);
		++i;
	}
}

void PlayerBattler::updateMagicMenuATBGain() {
	for (auto i = 0; i < battlerData->Abilities.size(); ++i) {
		auto aid = battlerData->Abilities[i];
		auto& ad = BattleSystem::GetAbilityByID(aid);
		if (ad.APCost < currentAP) continue;
		auto ac = ad.APCost <= currentAP;
		_battlerUI->UpdateMagicMenu(i, "", ac);
	}
}

void PlayerBattler::onAPGained() {
	_battlerUI->UpdateAP(to_string(currentAP));
	if (_isDead || currentHP <= 0) return;
	if (_currentBattlerState == BattlerStates::MagicSelection) {
		_battlerUI->UpdateAPCostCurrent(currentAP);
	}
	if (_currentBattlerState == BattlerStates::ATBCharging && !_reopenMenuAfterClose) {
		handleStateChange(ATBFullyCharged);
	}
	updateMagicMenuATBGain();
}

void PlayerBattler::handleStateChange(BattlerStates newState) {
	if ((newState == ATBCharging || newState == ATBFullyCharged || newState == TargetSelection) && shouldBattleEnd()) newState = BattleEndStart;
	switch (newState) {
		case BattlerStates::ATBCharging:
			popAllButtonGroups();
			_battlerUI->StartATBIdleAnim();
			_battlerUI->CloseCommandsMenu();
			_battlerUI->CloseTargetSelection();
			_battlerUI->EndPlayerTurn(this);
			break;
		case BattlerStates::ATBFullyCharged:
			sgLogWarn("Would take damage at start");
			_currentMenuLocation = 0;
			_battlerUI->StartATBTurnAnim();
			_battlerUI->OpenCommandsMenu();
			Engine::Audio::PlaySFXBuffer("playerTurn", 5.0f);
			_battlerUI->StartPlayerTurn(this);
			setupCommandButtonGroup();
			_commandButtonGroup->SetFocusedIndex(0);
			PointerInputSystem::PushGroup(_commandButtonGroup.get());
			break;
		case BattlerStates::MagicSelection:
			if (_commandButtonGroup) PointerInputSystem::PopGroup(_commandButtonGroup.get());
			_magicMenuRow = 0;
			_magicMenuCol = 0;
			_battlerUI->OpenMagicMenu();
			if (!battlerData->Abilities.empty()) {
				const auto& ability = BattleSystem::GetAbilityByID(battlerData->Abilities[0]);
				_battlerUI->ShowAPCostBox(currentAP, ability.APCost);
				_battlerUI->ShowMagicDescription(ability.Description);
			}
			setupMagicButtonGroup();
			_magicButtonGroup->SetFocusedIndex(0);
			PointerInputSystem::PushGroup(_magicButtonGroup.get());
			break;
		case BattlerStates::TargetSelection: {
			if (_magicButtonGroup) PointerInputSystem::PopGroup(_magicButtonGroup.get());
			if (_commandButtonGroup) PointerInputSystem::PopGroup(_commandButtonGroup.get());
			_currentTargetBattler = 0;
			_battlerUI->StartTargetSelection();
			moveFingerToTargetNum(0);
			setupTargetButtonGroup();
			_targetButtonGroup->SetFocusedIndex(0);
			PointerInputSystem::PushGroupRect(_targetButtonGroup.get());
			break;
		}
		case BattlerStates::BattleEndStart:
			popAllButtonGroups();
			Engine::Audio::PlayBGM("victory");
			_battlerUI->CloseCommandsMenu();
			_battlerUI->CloseTargetSelection();
			_battlerUI->ClosePlayerInfoBox();
			animator->StartAnimation(VICTORY_STR, -1);
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
				animator->UpdateAnimatorSpeed(0.0f);
			}
		}
		return;
	}
	if (currentHP <= 0) return;
	if (_isPlayingDamageAnim) {
		_damageAnimTimer -= GameState::DeltaTimeSeconds;
		if (_damageAnimTimer <= 0) {
			_isPlayingDamageAnim = false;
		}
	}

	if (_currentBattlerState != BattleEndStart &&
		_currentBattlerState != BattleEndIdle &&
		_currentBattlerState != BattlerStates::BattleSpoils &&
		_currentBattlerState != BattlerStates::BattleEnd &&
		shouldBattleEnd()) {
		handleStateChange(BattleEndStart);
	}
	switch (_currentBattlerState) {
		case BattlerStates::Default:
			handleStateChange(ATBCharging);
			break;
		case BattlerStates::ATBCharging: {
			auto progress = currentATBCharge / maxATBCharge * 100.00f;
			_battlerUI->UpdateProgressBar(progress);
			if (_reopenMenuAfterClose && _battlerUI->IsMenuClosed()) {
				_reopenMenuAfterClose = false;
				handleStateChange(ATBFullyCharged);
			}
			break;
		}
		case ATBFullyCharged: {
			handleStateChange(CommandSelection);
			break;
		}
		case CommandSelection:
		case MagicSelection:
		case TargetSelection: {
			auto progress = currentATBCharge / maxATBCharge * 100.00f;
			_battlerUI->UpdateProgressBar(progress);
			if (_currentBattlerState == TargetSelection) {
				std::vector<Battler*> targets;
				getAllTargets(targets);
				if (!targets.empty()) {
					moveFingerToTargetNum(_currentTargetBattler);
				}
			}
			break;
		}
		case BattleEndStart:
			handleStateChange(BattleEndIdle);
			break;
		default:
			break;
	}
	handleInput();
}

void PlayerBattler::takeDamageImpl(int damage) {
	_battlerUI->UpdateHP(to_string(currentHP));
	if (currentHP <= 0) {
		Engine::Audio::PlaySFXBuffer("hit2", 1.0f);
		animator->StartAnimation("dead1", -1);
		_isDead = true;
		_deathHoldTimer = animator->GetAnimationDuration("dead1");
		_isPlayingDamageAnim = false;
		_battlerUI->CloseCommandsMenu();
		_battlerUI->CloseTargetSelection();
		_battlerUI->EndPlayerTurn(this);
		return;
	}
	if (!_isPlayingDamageAnim) {
		_isPlayingDamageAnim = true;
		_damageAnimTimer = 0.5f;
		animator->PlayAnimationThenLoopSecond("damage1", battlerData->IdleAnimation);
	}
}

void PlayerBattler::healImpl(int amount) {
	_battlerUI->UpdateHP(to_string(currentHP));
}

void PlayerBattler::handleInputCommandsMenu() {
	auto newLocation = _currentMenuLocation;
	if (_controller->IsButtonJustPressed(ControllerButtons::Up)) {
		--newLocation;
	} else if (_controller->IsButtonJustPressed(ControllerButtons::Down)) {
		++newLocation;
	} else if (_controller->IsButtonJustPressed(ControllerButtons::A)) {
		activateCommandItem(_currentMenuLocation);
		return;
	}

	if (newLocation != _currentMenuLocation) {
		_currentMenuLocation = newLocation > 3 ? _currentMenuLocation == 3 ? 0 : 3 : newLocation;
		_battlerUI->MoveCursorInMenu(_currentMenuLocation);
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		if (_commandButtonGroup) _commandButtonGroup->SetFocusedIndex(_currentMenuLocation);
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
	if (_controller->IsButtonJustPressed(ControllerButtons::Up)) {
		if (newRow > 0) --newRow;
	} else if (_controller->IsButtonJustPressed(ControllerButtons::Down)) {
		if (newRow < 3) ++newRow;
	} else if (_controller->IsButtonJustPressed(ControllerButtons::Left)) {
		if (newCol > 0) --newCol;
	} else if (_controller->IsButtonJustPressed(ControllerButtons::Right)) {
		if (newCol < 1) ++newCol;
	} else if (_controller->IsButtonJustPressed(ControllerButtons::A)) {
		int magicIndex = _magicMenuCol * 4 + _magicMenuRow;
		activateMagicItem(magicIndex);
		return;
	} else if (_controller->IsButtonJustPressed(ControllerButtons::B)) {
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		if (_magicButtonGroup) PointerInputSystem::PopGroup(_magicButtonGroup.get());
		_battlerUI->CloseMagicMenu();
		_currentBattlerState = CommandSelection;
		setupCommandButtonGroup();
		_commandButtonGroup->SetFocusedIndex(_currentMenuLocation);
		PointerInputSystem::PushGroup(_commandButtonGroup.get());
		return;
	}

	if (newRow != _magicMenuRow || newCol != _magicMenuCol) {
		_magicMenuRow = newRow;
		_magicMenuCol = newCol;
		_battlerUI->MoveCursorInMagicMenu(_magicMenuCol, _magicMenuRow);
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		int slotIndex = _magicMenuCol * 4 + _magicMenuRow;
		if (slotIndex < (int)battlerData->Abilities.size()) {
			int abilityID = battlerData->Abilities[slotIndex];
			const auto& ab = BattleSystem::GetAbilityByID(abilityID);
			_battlerUI->ShowAPCostBox(currentAP, ab.APCost);
			_battlerUI->ShowMagicDescription(ab.Description);
		} else {
			_battlerUI->HideAPCostBox();
			_battlerUI->ShowMagicDescription("");
		}
		if (_magicButtonGroup) _magicButtonGroup->SetFocusedIndex(slotIndex);
	}
}

void PlayerBattler::handleInputTargetSelection() {
	int newTarget = _currentTargetBattler;

	if (_controller->IsButtonJustPressed(ControllerButtons::Up)) {
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		--newTarget;
	} else if (_controller->IsButtonJustPressed(ControllerButtons::Down)) {
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		++newTarget;
	} else if (_controller->IsButtonJustPressed(ControllerButtons::Left) ||
			   _controller->IsButtonJustPressed(ControllerButtons::Right)) {
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		_targetingFriendly = !_targetingFriendly;
		_currentTargetBattler = 0;
		moveFingerToTargetNum(0);
		if (_targetButtonGroup) PointerInputSystem::PopGroupRect(_targetButtonGroup.get());
		setupTargetButtonGroup();
		_targetButtonGroup->SetFocusedIndex(0);
		PointerInputSystem::PushGroupRect(_targetButtonGroup.get());
		return;
	} else if (_controller->IsButtonJustPressed(ControllerButtons::A)) {
		activateTarget(_currentTargetBattler);
		return;
	} else if (_controller->IsButtonJustPressed(ControllerButtons::B)) {
		Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
		if (_targetButtonGroup) PointerInputSystem::PopGroupRect(_targetButtonGroup.get());
		_battlerUI->CloseTargetSelection();
		if (_selectedAbilityID > 0) {
			_currentBattlerState = MagicSelection;
			setupMagicButtonGroup();
			int slotIndex = _magicMenuCol * 4 + _magicMenuRow;
			_magicButtonGroup->SetFocusedIndex(slotIndex);
			PointerInputSystem::PushGroup(_magicButtonGroup.get());
			if (slotIndex < (int)battlerData->Abilities.size()) {
				int abilityID = battlerData->Abilities[slotIndex];
				const auto& ab = BattleSystem::GetAbilityByID(abilityID);
				_battlerUI->ShowAPCostBox(currentAP, ab.APCost);
				_battlerUI->ShowMagicDescription(ab.Description);
			}
		} else {
			_currentBattlerState = CommandSelection;
			setupCommandButtonGroup();
			_commandButtonGroup->SetFocusedIndex(_currentMenuLocation);
			PointerInputSystem::PushGroup(_commandButtonGroup.get());
		}
		return;
	}

	if (newTarget != _currentTargetBattler) {
		moveFingerToTargetNum(newTarget);
		if (_targetButtonGroup) _targetButtonGroup->SetFocusedIndex(_currentTargetBattler);
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
			if (_controller->IsButtonJustPressed(ControllerButtons::A)) {
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

void PlayerBattler::activateCommandItem(int index) {
	switch (index) {
		case 0: {
			const auto& ability = BattleSystem::GetAbilityByID(0);
			if (currentAP < ability.APCost) {
				Engine::Audio::PlaySFXBuffer("error1", 1.0f);
				return;
			}
			Engine::Audio::PlaySFXBuffer("menuSelect", 1.0f);
			_selectedAbilityID = 0;
			_targetingFriendly = false;
			handleStateChange(TargetSelection);
			break;
		}
		case 1:
			Engine::Audio::PlaySFXBuffer("menuSelect", 1.0f);
			handleStateChange(MagicSelection);
			break;
		default:
			sgLogDebug("Button not implemented", index);
			Engine::Audio::PlaySFXBuffer("error1", 1.0f);
			break;
	}
}

void PlayerBattler::activateMagicItem(int index) {
	int row = index % 4;
	int col = index / 4;
	int slotIndex = col * 4 + row;
	if (slotIndex >= (int)battlerData->Abilities.size()) {
		Engine::Audio::PlaySFXBuffer("error1", 1.0f);
		return;
	}
	_selectedAbilityID = battlerData->Abilities[slotIndex];
	const auto& ability = BattleSystem::GetAbilityByID(_selectedAbilityID);
	if (currentAP < ability.APCost) {
		Engine::Audio::PlaySFXBuffer("error1", 1.0f);
		return;
	}
	Engine::Audio::PlaySFXBuffer("menuSelect", 1.0f);
	_targetingFriendly = ability.Friendly;
	_battlerUI->HideAPCostBox();
	handleStateChange(TargetSelection);
}

void PlayerBattler::activateTarget(int index) {
	Engine::Audio::PlaySFXBuffer("menuSelect", 1.0f);
	vector<Battler*> targets;
	getAllTargets(targets);
	if (targets.empty() || index >= (int)targets.size()) return;
	_currentTargetBattler = index;
	const auto battler = targets.at(index);
	const auto& ability = BattleSystem::GetAbilityByID(_selectedAbilityID);
	const auto& playerAnim = ability.PlayerAnim.empty() ? "slash2" : ability.PlayerAnim;
	animator->PlayAnimationThenLoopSecond(playerAnim, battlerData->IdleAnimation);
	if (battler) {
		if (ability.BaseDamage < 0) {
			battler->Heal(-ability.BaseDamage);
		} else {
			battler->TakeDamage(ability.BaseDamage + GetOutgoingDamageBonus());
		}
		for (auto& se : ability.StatusEffects) {
			auto sed = static_cast<StatusEffects>(se.Id);
			StatusEffectInstance sei = {sed, 2};
			battler->ApplyStatusEffect(sei);
		}
		battler->PlayHitAnimation(ability);
	}
	spendAP(ability.APCost);
	_battlerUI->UpdateAP(to_string(currentAP));
	currentATBCharge = 0;
	_reopenMenuAfterClose = currentAP > 0;
	handleTurnEndStatus();
	handleStateChange(ATBCharging);
}

void PlayerBattler::setupCommandButtonGroup() {
	_commandButtonGroup = std::make_unique<UIButtonGroup>(
		[this](int index) {
			_currentMenuLocation = index;
			_battlerUI->MoveCursorInMenu(index);
			Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
			_commandButtonGroup->SetFocusedIndex(index);
		},
		[this](int index) {
			activateCommandItem(index);
		});
	for (int i = 0; i < 4; ++i) {
		auto* item = _battlerUI->GetMenuItem(i);
		if (item) _commandButtonGroup->AddButton(item, i);
	}
}

void PlayerBattler::setupMagicButtonGroup() {
	_magicButtonGroup = std::make_unique<UIButtonGroup>(
		[this](int index) {
			int row = index % 4;
			int col = index / 4;
			_magicMenuRow = row;
			_magicMenuCol = col;
			_battlerUI->MoveCursorInMagicMenu(col, row);
			Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
			int slotIndex = col * 4 + row;
			if (slotIndex < (int)battlerData->Abilities.size()) {
				int abilityID = battlerData->Abilities[slotIndex];
				const auto& ab = BattleSystem::GetAbilityByID(abilityID);
				_battlerUI->ShowAPCostBox(currentAP, ab.APCost);
				_battlerUI->ShowMagicDescription(ab.Description);
			} else {
				_battlerUI->HideAPCostBox();
				_battlerUI->ShowMagicDescription("");
			}
			_magicButtonGroup->SetFocusedIndex(index);
		},
		[this](int index) {
			activateMagicItem(index);
		});
	for (int i = 0; i < 8; ++i) {
		auto* item = _battlerUI->GetMagicMenuItem(i);
		if (item) _magicButtonGroup->AddButton(item, i);
	}
}

void PlayerBattler::setupTargetButtonGroup() {
	_targetButtonGroup = std::make_unique<UIButtonGroupRect>(
		[this](int index) {
			moveFingerToTargetNum(index);
			Engine::Audio::PlaySFXBuffer("menuMove", 1.0f);
			_targetButtonGroup->SetFocusedIndex(index);
		},
		[this](int index) {
			activateTarget(index);
		});

	vector<Battler*> targets;
	getAllTargets(targets);
	float camX = CameraGetX();
	float camY = CameraGetY();
	for (int i = 0; i < (int)targets.size(); ++i) {
		auto* t = targets[i];
		if (!t) continue;
		RectangleF rect = {
			t->SpriteX() - camX,
			t->SpriteY() - camY,
			t->SpriteWidth(),
			t->SpriteHeight()};
		_targetButtonGroup->AddButton(rect, i);
	}
}

void PlayerBattler::popAllButtonGroups() {
	if (_commandButtonGroup) PointerInputSystem::PopGroup(_commandButtonGroup.get());
	if (_magicButtonGroup) PointerInputSystem::PopGroup(_magicButtonGroup.get());
	if (_targetButtonGroup) PointerInputSystem::PopGroupRect(_targetButtonGroup.get());
}

void PlayerBattler::handleClickAction() {
	// Placeholder — button groups handle clicks via PointerInputSystem callbacks
}
