#include <Supergoon/state.h>
#include <sgtools/log.h>

#include <battle/battlerUI.hpp>
#include <engine.hpp>
#include <format>
#include <gameobject/gameobjects/Battler.hpp>
#include <ui/ui.hpp>
#include <ui/uiNineSlice.hpp>
#include <ui/uiText.hpp>

#include "ui/uiAnimation.hpp"
using namespace Etf;
using namespace std;
using enum PlayerUIAnimationStates;

namespace {
const float Animation_Offset = 120.0f;
const float Animation_Open_Time = 0.15f;
const float Animation_Close_Time = 0.10;
}  // namespace

UIAnimation* BattlerUI::_turnMarkerAnim = nullptr;

void BattlerUI::StartPlayerTurn(Battler* battler) {
	auto x = battler->X() + (battler->SpriteWidth() / 2) - 10;
	auto y = battler->Y() - 3;
	_turnMarkerAnim->AbsolutePosition(x, y);
	_turnMarkerAnim->SetVisible(true);
}

void BattlerUI::EndPlayerTurn(Battler* battler) {
	_turnMarkerAnim->SetVisible(false);
}

BattlerUI::BattlerUI(unsigned int battlerNum) : _battlerNum(battlerNum) {
	auto turnMarker = UI::GetRootUIObject()->GetChildByName("TurnMarker");
	if (!turnMarker) sgLogCritical("Could not find turnMarker, exiting");
	_turnMarkerAnim = static_cast<UIAnimation*>(turnMarker);
	_turnMarkerAnim->SetVisible(false);
	_turnMarkerAnim->GetAnimator().StartAnimation("playing");

	_player = battlerNum < 3;
	if (_player) {
		auto playerUIName = format("Player{}CommandsUI", to_string(battlerNum + 1));
		auto playerRoot = UI::GetRootUIObject()->GetChildByName(playerUIName);
		if (!playerRoot) sgLogCritical("Could not find %s, exiting", playerUIName.c_str());

		_commandMenu = playerRoot->GetChildByName("CommandsNineSlice");
		if (!_commandMenu) {
			sgLogCritical("Could not find child CommandsNineSlice, exiting");
		}
		_menuBoxStartX = _commandMenu->OriginalX();
		_menuBoxStartY = _commandMenu->OriginalY();
		_commandMenu->SetVisible(false);
		_commandMenu->SetX(_menuBoxStartX + Animation_Offset);
		auto vlg = playerRoot->GetChildByName("CommandsVLG");
		if (!vlg) {
			sgLogCritical("Could not find child CommandsVLG, exiting");
		}
		_menuItems[0] = vlg->GetChildByName("AttackText");
		_menuItems[1] = vlg->GetChildByName("MagicText");
		_menuItems[2] = vlg->GetChildByName("SkillsText");
		_menuItems[3] = vlg->GetChildByName("ItemsText");
		Color disabledColor = {180, 180, 180, 255};
		for (int i = 0; i < 4; ++i) {
			if (!_menuItems[i])
				sgLogCritical("Could not find child menu item, exiting");
			if (i >= 2) {
				auto menuCasted = static_cast<UIText*>(_menuItems[i]);
				if (menuCasted) menuCasted->UpdateDrawColor(disabledColor);
			}
		}
		auto finger = playerRoot->GetChildByName("Finger");
		if (finger) {
			_finger = static_cast<UIImage*>(finger);
		}
		finger->SetVisible(false);
		auto tsFinger = playerRoot->GetChildByName("BattleSelectionFinger");
		if (tsFinger) {
			_targetSelectionFinger = static_cast<UIImage*>(tsFinger);
		}
		_targetSelectionFinger->SetVisible(false);

		if (battlerNum == 1) {
			Color p2Color = {100, 255, 180, 255};
			if (_finger) _finger->UpdateDrawColor(p2Color);
			if (_targetSelectionFinger) _targetSelectionFinger->UpdateDrawColor(p2Color);
		}

		auto hpName = format("P{}Health", to_string(battlerNum + 1));
		auto hpObject = UI::GetRootUIObject()->GetChildByName(hpName);
		_hpObject = static_cast<UIText*>(hpObject);

		auto atbName = format("P{}ATB", to_string(battlerNum + 1));
		auto progressBarAnim = UI::GetRootUIObject()->GetChildByName(atbName);
		_progressBarAnim = static_cast<UIAnimation*>(progressBarAnim);
		if (!progressBarAnim) sgLogCritical("Could not find progress bar anim, exiting");
		StartATBIdleAnim();

		auto atbBarName = format("P{}ATBProgressBar", to_string(battlerNum + 1));
		auto progressBarObject = UI::GetRootUIObject()->GetChildByName(atbBarName);
		if (!progressBarObject) sgLogCritical("Could not find progress bar, exiting");
		_progressBar = static_cast<UIProgressBar*>(progressBarObject);

		UINineSliceArgs infoBoxArgs;
		infoBoxArgs.Filename = "uibase";
		infoBoxArgs.Name = "TargetInfoBox";
		infoBoxArgs.Rect = {5, 5, 90, 32};
		infoBoxArgs.SourceRect = {0, 0, 64, 64};
		infoBoxArgs.Xoffset = 8;
		infoBoxArgs.Yoffset = 8;
		infoBoxArgs.Scale = 1.0f;
		infoBoxArgs.DrawColor = {80, 0, 120, 235};
		infoBoxArgs.Priority = 2;
		infoBoxArgs.Visible = false;
		_targetInfoBox = new UINineSlice(infoBoxArgs);
		UI::GetRootUIObject()->GetChildByName("BattlePanel")->AddChild(_targetInfoBox);

		UITextArgs infoTextArgs;
		infoTextArgs.FontName = "PressStart2P";
		infoTextArgs.FontSize = 8;
		infoTextArgs.Rect = {6, 6, 78, 20};
		infoTextArgs.TextToDraw = "";
		infoTextArgs.Name = "TargetInfoText";
		infoTextArgs.NumCharsToDraw = 100;
		infoTextArgs.Priority = 3;
		infoTextArgs.TextColor = {255, 255, 255, 255};
		infoTextArgs.CenteredX = true;
		infoTextArgs.CenteredY = true;
		infoTextArgs.WordWrap = false;
		infoTextArgs.Visible = true;
		infoTextArgs.DebugBox = false;
		_targetInfoText = new UIText(infoTextArgs);
		_targetInfoBox->AddChild(_targetInfoText);

		_magicMenu = playerRoot->GetChildByName("MagicNineSlice");
		if (_magicMenu) {
			_magicMenuStartX = _magicMenu->OriginalX();
			_magicMenu->SetVisible(false);
			_magicMenu->SetX(_magicMenuStartX + Animation_Offset);
		}
		for (int i = 0; i < 8; ++i) {
			_magicMenuItems[i] = playerRoot->GetChildByName("MagicSlot" + to_string(i));
		}
		auto magicFingerObj = playerRoot->GetChildByName("MagicFinger");
		if (magicFingerObj) {
			_magicFinger = static_cast<UIImage*>(magicFingerObj);
			_magicFinger->SetVisible(false);
		}
	}
}

BattlerUI::~BattlerUI() {
	if (!_player && _hpObject) {
		_hpObject->SetVisible(false);
	}
}

void BattlerUI::UpdateProgressBar(float percent) {
	if (!_progressBar) return;
	_progressBar->SetBarPercent(percent);
}

void BattlerUI::StartTargetSelection() {
	_targetSelectionFinger->SetVisible(true);
	if (_targetInfoBox) _targetInfoBox->SetVisible(true);
}

void BattlerUI::CloseTargetSelection() {
	_targetSelectionFinger->SetVisible(false);
	if (_targetInfoBox) _targetInfoBox->SetVisible(false);
}

void BattlerUI::UpdateTargetInfo(const std::string& displayName) {
	if (_targetInfoText) _targetInfoText->UpdateText(displayName);
}

void BattlerUI::UpdateHP(const string& hp) {
	if (!_hpObject) return;
	_hpObject->UpdateText(hp);
}

void BattlerUI::UpdateAnimations() {
	switch (_currentState) {
		case Closed:
			break;
		case Opened:
			break;
		case Opening: {
			_currentAnimationTime += DeltaTimeSeconds;
			if (_currentAnimationTime >= Animation_Open_Time) {
				_currentState = Opened;
				_finger->SetVisible(true);
				break;
			}
			auto newX = Engine::Tweening::GetTweenedValue(_menuBoxStartX + Animation_Offset, _menuBoxStartX, _currentAnimationTime, Animation_Open_Time, Engine::Tweening::TweenEaseTypes::QuintOut);
			_commandMenu->SetX(newX);
			break;
		}
		case Closing: {
			_currentAnimationTime += DeltaTimeSeconds;
			if (_currentAnimationTime > Animation_Open_Time) {
				_commandMenu->SetVisible(false);
				_currentState = Closed;
				break;
			}
			auto newX = Engine::Tweening::GetTweenedValue(_menuBoxStartX, _menuBoxStartX + Animation_Offset, _currentAnimationTime, Animation_Close_Time, Engine::Tweening::TweenEaseTypes::QuintOut);
			_commandMenu->SetX(newX);
			break;
		}
		default:
			break;
	}
	if (_magicMenu) {
		switch (_magicMenuState) {
			case Closed:
				break;
			case Opened:
				break;
			case Opening: {
				_magicAnimationTime += DeltaTimeSeconds;
				if (_magicAnimationTime >= Animation_Open_Time) {
					_magicMenuState = Opened;
					_magicMenu->SetX(_magicMenuStartX);
					MoveCursorInMagicMenu(_pendingMagicCol, _pendingMagicRow);
					if (_magicFinger) _magicFinger->SetVisible(true);
					break;
				}
				auto newX = Engine::Tweening::GetTweenedValue(_magicMenuStartX + Animation_Offset, _magicMenuStartX, _magicAnimationTime, Animation_Open_Time, Engine::Tweening::TweenEaseTypes::QuintOut);
				_magicMenu->SetX(newX);
				break;
			}
			case Closing: {
				_magicAnimationTime += DeltaTimeSeconds;
				if (_magicAnimationTime > Animation_Close_Time) {
					_magicMenu->SetVisible(false);
					_magicMenuState = Closed;
					break;
				}
				auto newX = Engine::Tweening::GetTweenedValue(_magicMenuStartX, _magicMenuStartX + Animation_Offset, _magicAnimationTime, Animation_Close_Time, Engine::Tweening::TweenEaseTypes::QuintOut);
				_magicMenu->SetX(newX);
				break;
			}
			default:
				break;
		}
	}
}

void BattlerUI::MoveCursorInMenu(unsigned int newLocation) {
	if (!_player || _currentState != Opened) return;
	auto uiobject = _menuItems[newLocation];
	if (!uiobject) sgLogCritical("No menu item to switch to");
	auto thing = uiobject->AbsolutePosition();
	auto x = thing.x - 15;
	auto y = thing.y + (15 * newLocation);
	_finger->AbsolutePosition(x, y);
}

void BattlerUI::OpenMagicMenu() {
	if (!_magicMenu) return;
	_magicMenu->SetVisible(true);
	_magicMenuState = PlayerUIAnimationStates::Opening;
	_magicAnimationTime = 0;
	_pendingMagicCol = 0;
	_pendingMagicRow = 0;
}

void BattlerUI::CloseMagicMenu() {
	if (!_magicMenu) return;
	if (_magicMenuState == PlayerUIAnimationStates::Closed) return;
	_magicMenuState = PlayerUIAnimationStates::Closing;
	_magicAnimationTime = 0;
	if (_magicFinger) _magicFinger->SetVisible(false);
}

void BattlerUI::MoveCursorInMagicMenu(unsigned int col, unsigned int row) {
	if (!_magicFinger || !_magicMenu) return;
	if ((int)col >= _magicMenuCols || (int)row >= _magicMenuRows) return;
	auto uiobject = _magicMenuItems[0];
	if (!uiobject) return;
	auto pos = uiobject->AbsolutePosition();
	auto x = pos.x - 15 + (50 * col);
	auto y = pos.y + (15 * row);
	_magicFinger->AbsolutePosition(x, y);
}

void BattlerUI::MoveFingerToBattlerLocation(Battler* battler) {
	auto x = battler->X() + (battler->SpriteWidth() / 2) - 5;
	auto y = battler->Y() - 10;
	if (_battlerNum == 1) {
		x += 12;
		y += 8;
	}
	_targetSelectionFinger->AbsolutePosition(x, y);
}

void BattlerUI::ClosePlayerInfoBox() {
	auto menu = UI::GetRootUIObject()->GetChildByName("PlayerStatusHUD");
	if (menu) {
		menu->SetVisible(false);
	}
}
