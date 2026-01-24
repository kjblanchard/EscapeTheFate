#include <Supergoon/log.h>

#include <battle/battlerUI.hpp>
#include <ui/ui.hpp>
using namespace Etf;
using namespace std;

BattlerUI::BattlerUI(unsigned int battlerNum) {
	_player = battlerNum < 3;
	if (_player) {
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
		auto hpObject = UI::RootUIObject->GetChildByName("P1Health");
		_hpObject = static_cast<UIText*>(hpObject);
	} else {
		auto hpObject = UI::RootUIObject->GetChildByName("EnemyHP");
		_hpObject = static_cast<UIText*>(hpObject);
	}
}

void BattlerUI::UpdateHP(const string& hp) {
	if (!_hpObject) return;
	_hpObject->UpdateText(hp);
}

void BattlerUI::MoveCursorInMenu(unsigned int newLocation) {
	if (!_player) return;
	auto uiobject = _menuItems[newLocation];
	if (!uiobject) sgLogCritical("No menu item to switch to");
	auto thing = uiobject->GetAbsolutePosition();
	auto x = thing.x - 8;
	auto y = thing.y + (15 * newLocation) + 8;
	sgLogDebug("Moving to %d, Setting _finger to position %f, %f from: %f %f", newLocation, (double)x, (double)y, (double)_finger->X(), (double)_finger->Y());
	_finger->SetAbsolutePosition(x, y);
}
