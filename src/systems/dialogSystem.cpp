#include <Supergoon/log.h>

#include <systems/dialogSystem.hpp>
#include <ui/ui.hpp>
#include <ui/uiText.hpp>

#include "ui/uiObject.hpp"
using namespace Etf;
using namespace std;

string DialogSystem::_currentText;
UIObject* DialogSystem::_dialogBoxObject;
UIText* DialogSystem::_dialogBoxTextObject;

void DialogSystem::UpdateDialogText(const std::string& newText, int lettersToDisplay) {
	if (!_dialogBoxObject) {
		_dialogBoxObject = UI::RootUIObject->GetChildByName("DialogBox");
	}
	if (_dialogBoxObject && !_dialogBoxTextObject) {
		_dialogBoxTextObject = (UIText*)_dialogBoxObject->GetChildByName("DialogBoxText");
	}
	if (!_dialogBoxObject || !_dialogBoxTextObject) {
		sgLogWarn("Not updating dialog box, could not find dialog box or text");
		return;
	}
	_dialogBoxTextObject->UpdateText(newText);
}
