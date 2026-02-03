#include <Supergoon/log.h>
#include <Supergoon/state.h>

#include <battle/battlerUI.hpp>
#include <bindings/engine.hpp>
#include <gameobject/gameobjects/Battler.hpp>
#include <ui/ui.hpp>
using namespace Etf;
using namespace std;
using enum PlayerUIAnimationStates;

static const float Animation_Offset = 120.0f;
static const float Animation_Open_Time = 0.15f;
static const float Animation_Close_Time = 0.10;

BattlerUI::BattlerUI(unsigned int battlerNum) {
	_player = battlerNum < 3;
	if (_player) {
		_commandMenu = UI::RootUIObject->GetChildByName("CommandsNineSlice");
		if (!_commandMenu) {
			sgLogCritical("Could not fild child CommandsNineSlice, exiting");
		}
		_menuBoxStartX = _commandMenu->OriginalX();
		_menuBoxStartY = _commandMenu->OriginalY();
		_commandMenu->SetVisible(false);
		_commandMenu->SetX(_menuBoxStartX + Animation_Offset);
		auto vlg = UI::RootUIObject->GetChildByName("CommandsVLG");
		if (!vlg) {
			sgLogCritical("Could not fild child commandsvlg, exiting");
		}
		_menuItems[0] = vlg->GetChildByName("AttackText");
		_menuItems[1] = vlg->GetChildByName("MagicText");
		_menuItems[2] = vlg->GetChildByName("SkillsText");
		_menuItems[3] = vlg->GetChildByName("ItemsText");
		Color color = {180, 180, 180, 255};
		auto first = true;
		for (auto menuItem : _menuItems) {
			if (!menuItem)
				sgLogCritical("Could not fild child thing, exiting");
			auto menuCasted = static_cast<UIText*>(menuItem);
			if (!menuCasted) continue;
			if (first) {
				first = false;
			} else {
				menuCasted->UpdateDrawColor(color);
			}
		}
		auto finger = UI::RootUIObject->GetChildByName("Finger");
		if (finger) {
			_finger = static_cast<UIImage*>(finger);
		}
		finger->SetVisible(false);
		auto ui = UI::RootUIObject->GetChildByName("Player1CommandsUI");
		auto tsFinger = ui->GetChildByName("BattleSelectionFinger");
		if (tsFinger) {
			_targetSelectionFinger = static_cast<UIImage*>(tsFinger);
		}
		_targetSelectionFinger->SetVisible(false);
		auto hpObject = UI::RootUIObject->GetChildByName("P1Health");
		_hpObject = static_cast<UIText*>(hpObject);
		auto progressBarAnim = UI::RootUIObject->GetChildByName("P1ATB");
		_progressBarAnim = static_cast<UIAnimation*>(progressBarAnim);
		if (!progressBarAnim) sgLogCritical("Could not find progress bar anim, exiting");
		StartATBIdleAnim();
		auto progressBarObject = UI::RootUIObject->GetChildByName("P1ATBProgressBar");
		if (!progressBarObject) sgLogCritical("Could not find progress bar, exiting");
		_progressBar = static_cast<UIProgressBar*>(progressBarObject);
	} else {
		auto hpObject = UI::RootUIObject->GetChildByName("EnemyHP");
		_hpObject = static_cast<UIText*>(hpObject);
		if (_hpObject) _hpObject->SetVisible(true);
	}
}

BattlerUI::~BattlerUI() {
	if (!_player && _hpObject) {
		_hpObject->SetVisible(false);
	}
}

void BattlerUI::UpdateProgressBar(float percent) {
	_progressBar->SetBarPercent(percent);
}

void BattlerUI::StartTargetSelection() {
	_targetSelectionFinger->SetVisible(true);
}

void BattlerUI::CloseTargetSelection() {
	_targetSelectionFinger->SetVisible(false);
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
}

void BattlerUI::MoveCursorInMenu(unsigned int newLocation) {
	if (!_player || _currentState != Opened) return;
	auto uiobject = _menuItems[newLocation];
	if (!uiobject) sgLogCritical("No menu item to switch to");
	auto thing = uiobject->GetAbsolutePosition();
	auto x = thing.x - 15;
	auto y = thing.y + (15 * newLocation);
	_finger->SetAbsolutePosition(x, y);
}

void BattlerUI::MoveFingerToBattlerLocation(Battler* battler) {
	auto x = battler->X() + (battler->SpriteWidth() / 2) - 5;
	auto y = battler->Y() - 10;
	_targetSelectionFinger->SetAbsolutePosition(x, y);
}

void BattlerUI::ClosePlayerInfoBox() {
	auto menu = UI::RootUIObject->GetChildByName("PlayerStatusHUD");
	if (menu) {
		menu->SetVisible(false);
	}
}
